#ifndef PRODUCERCONSUMER_H_
#define PRODUCERCONSUMER_H_

#include <assert.h>

/*******************************************************************************
 * This class creates a producer-consumer context, with a limited number of
 * items to be stored in a queue like manner.
 * Items are returned in the order they where produced.
 * Production of several items may occur in parallel. The order is given by the
 * release() call (closing an item out of production).
 * Consumption of several items may occur in parallel.
 * ****************************************************************************/
template<class T, int ItemNumber, class Semaphore>
class ProducerConsumer
{
public:
  ProducerConsumer();
  virtual ~ProducerConsumer();

  /**
   * This procedure produces an item in/from the producer-consumer context and returns
   * its reference.
   *
   * @note This call blocks always on unreleased old produced items, which will never happen,
   * if the producer is single threaded.
   *
   * \param  block    with true: wait, until some item storage becomes available,
   *                  with false: overwrite the oldest item
   *
   * \return          a reference to the item in production
   */
  T& produce(bool block = true);

  /**
   * This procedure releases a produced item to the world for consumption.
   *
   * \param product   is a reference to the new product, that is to be released to
   *                  the consumer world. The address of the object _MUST_ be the
   *                  same as the address of the object returned by produce()
   */
  void release(T& product);

  /**
   * This procedure returns the number of clean slots in the ring buffer
   */
  int slotsAvailable(void);

  /**
   * This procedure grabs an item in the group for consumption.
   *
   * \param block     with true, wait, until some item becomes available for consumption
   *                  with false, return NULL pointer in case, no item is available immediately
   *
   * \return          a pointer to the item in consumption
   */
  T* consume(bool block = true);

  /**
   * This procedure cleans a consumed item and makes the item free for next production.
   *
   * \param object    points to the consumed object to be cleaned up and returned to production space.
   */
  void cleanup(T* object);

  /**
   * This procedure returns the number of produced and released items in the ring buffer
   */
  int itemsAvailable(void);

  /**
   * The interface using the index operator to the ring buffer elements is
   * intended for initialization access to the elements, beside the managed
   * production/consumption interface
   */
  static const int NumBufferItems = ItemNumber;
  T& operator[](unsigned int idx);

private:
  struct Item : public T
  {
    bool inProduction;
    bool inConsumption;
    bool inUse;
  };
  Item itemBuffer[ItemNumber];
  Item* freeBuffer[ItemNumber];
  Item* releasedBuffer[ItemNumber];

  int numProducedItems, numReleasedItems;
  Semaphore free;       ///< this counts the free items in the buffer
  Semaphore item;       ///< this counts the waiting items in the buffer
  Semaphore mutex;      ///< this locks the admin data for the buffer
  int nextAdd, nextRelease, nextConsume, nextRemoved;

};

template<class T, int ItemNumber, class Semaphore>
ProducerConsumer<T, ItemNumber, Semaphore>::ProducerConsumer()
{
  mutex.init(1);
  mutex.acquire();
  nextAdd = 0;
  nextRelease = 0;
  nextConsume = 0;
  nextRemoved = 0;
  numProducedItems = 0;
  numReleasedItems = 0;
  free.init(ItemNumber);
  item.init(0);
  int i;
  for (i = 0; i < ItemNumber; i++)
  {
    itemBuffer[i].inConsumption = false;
    itemBuffer[i].inProduction = false;
    itemBuffer[i].inUse = false;

    freeBuffer[i] = &itemBuffer[i];
    releasedBuffer[i] = 0;
  }
  mutex.release();
}

template<class T, int ItemNumber, class Semaphore>
ProducerConsumer<T, ItemNumber, Semaphore>::~ProducerConsumer()
{
}

template<class T, int ItemNumber, class Semaphore>
T& ProducerConsumer<T, ItemNumber, Semaphore>::produce(bool block)
{
  Item* product;

  mutex.acquire();
  if (!block && (ItemNumber == numProducedItems))
  {
    mutex.release();
    // this will eat up the oldest item, but block on unreleased and on uncleaned items
    item.acquire();
    mutex.acquire();
    product = releasedBuffer[nextConsume];
    assert(not product->inProduction);
    assert(not product->inConsumption);
    product->inUse = false;
    freeBuffer[nextRemoved] = product;
    releasedBuffer[nextConsume] = 0;
    ++nextConsume;
    nextConsume %= ItemNumber;
    ++nextRemoved;
    nextRemoved %= ItemNumber;
    --numProducedItems;
    --numReleasedItems;
    free.release();
  }
  mutex.release();

  free.acquire();
  mutex.acquire();
  product = freeBuffer[nextAdd];
  freeBuffer[nextAdd] = 0;
  assert(0 != product);                 // programming error, product was never put to free buffer
  assert(not product->inUse);
  assert(not product->inProduction);
  product->inProduction = true;
  product->inUse = true;
  ++nextAdd;
  nextAdd %= ItemNumber;
  ++numProducedItems;
  mutex.release();

  return *product;
}


template<class T, int ItemNumber, class Semaphore>
void ProducerConsumer<T, ItemNumber, Semaphore>::release(T& p)
{
  Item* product = static_cast<Item*>(&p);

  assert(product->inUse);           // got an object, that was not produced here?
  assert(product->inProduction);    // got an object, that was not produced here?

  mutex.acquire();
  product->inProduction = false;
  releasedBuffer[nextRelease] = product;
  ++nextRelease;
  nextRelease %= ItemNumber;
  ++numReleasedItems;
  mutex.release();
  item.release();
}

template<class T, int ItemNumber, class Semaphore>
T* ProducerConsumer<T, ItemNumber, Semaphore>::consume(bool block)
{
  Item* object;

  mutex.acquire();
  if (!block && (0 == numReleasedItems))
  {
    mutex.release();
    return 0;
  }
  mutex.release();

  item.acquire();
  mutex.acquire();
  object = releasedBuffer[nextConsume];
  releasedBuffer[nextConsume] = 0;
  assert(0 != object);                  // programming error, object was never set in released buffer, but returned here
  assert(object->inUse);                // programming error, object is not in use, but in the released buffer
  object->inConsumption = true;
  ++nextConsume;
  nextConsume %= ItemNumber;
  --numReleasedItems;
  mutex.release();

  return object;
}

template<class T, int ItemNumber, class Semaphore>
void ProducerConsumer<T, ItemNumber, Semaphore>::cleanup(T* o)
{
  Item* object = static_cast<Item*>(o);

  assert(0 != object);
  assert(object->inUse);
  assert(object->inConsumption);

  mutex.acquire();
  object->inUse = false;
  object->inConsumption = false;
  freeBuffer[nextRemoved] = object;
  ++nextRemoved;
  nextRemoved %= ItemNumber;
  --numProducedItems;
  mutex.release();
  free.release();
}

template<class T, int ItemNumber, class Semaphore>
int ProducerConsumer<T, ItemNumber, Semaphore>::slotsAvailable(void)
{
  int ret;

  mutex.acquire();
  ret = ItemNumber - numProducedItems;
  mutex.release();

  return ret;
}

template<class T, int ItemNumber, class Semaphore>
int ProducerConsumer<T, ItemNumber, Semaphore>::itemsAvailable(void)
{
  int ret;

  mutex.acquire();
  ret = numReleasedItems;
  mutex.release();

  return ret;
}

template<class T, int ItemNumber, class Semaphore>
T& ProducerConsumer<T, ItemNumber, Semaphore>::operator[](unsigned int idx)
{
  idx %= NumBufferItems;
  return itemBuffer[idx];
}
#endif /*PRODUCERCONSUMER_H_*/
