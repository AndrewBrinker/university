#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
class Vector {
 public:
  typedef T* iterator;

  Vector();
  Vector(const Vector &v);
  Vector(unsigned int n, T x);
  ~Vector();

  Vector & operator=(const Vector &v);
  T & operator[](unsigned int i);

  unsigned int capacity() const;
  unsigned int size() const;
  bool empty() const;
  void reserve(unsigned int min_cap);
  void resize(unsigned int min_size, T k = T());
  T& front() const;
  T& back() const;
  void pop_back();
  void push_back(T x);

  iterator begin() const;
  iterator end() const;

private:
  unsigned int _capacity;
  unsigned int _size;
  T *data;
};


/**
 * Default constructor. Initializes all internal variables to 0.
 */
template <typename T>
Vector<T>::Vector(): _capacity(0), _size(0), data(0) {}


/**
 * Copy constructor. Duplicates an existing Vector.
 * @param: v -> The Vector being copied.
 */
template <typename T>
Vector<T>::Vector(const Vector &v) {
  _capacity = v.capacity();
  _size = v.size();
  data = new T[_capacity];
  for (unsigned int i = 0; i < _size; ++i) {
    data[i] = v.data[i];
  }
}


/**
 * Fill constructor. Creates a new instances of size n with fill item x.
 * @param: n -> The size of the Vector
 * @param: x -> The filler item
 */
template <typename T>
Vector<T>::Vector(unsigned int n, T x): _capacity(n), _size(n) {
  data = new T[n];
  for (unsigned int i = 0; i < _size; ++i) {
    data[i] = x;
  }
}


/**
 * Destructor. Deletes the internal data.
 */
template <typename T>
Vector<T>::~Vector() {
  delete [] data;
}


/**
 * Assignment operator. Duplicates the internal data of the other Vector
 * @param: v -> The vector being assigned from.
 * @return a reference to the current object
 */
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector &v) {
  if (v.capacity() > _capacity) {
    _capacity = v.capacity();
  }
  else if (_capacity < v.size()) {
    _capacity = (v.size() + 1);
  }
  _size = v.size();
  data = new T[_capacity];
  for (unsigned int i = 0; i < _size; ++i) {
    data[i] = v.data[i];
  }
  return *this;
}


/**
 * Index operator.
 * @param: i -> The index being grabbed
 * @return a reference to the current object
 */
template <typename T>
T & Vector<T>::operator[](unsigned int i) {
  return data[i];
}


/**
 * Capacity accessor function
 * @return the capacity of the Vector
 */
template <typename T>
unsigned int Vector<T>::capacity() const {
    return _capacity;
}


/**
 * Size accessor function
 * @return the size of the Vector
 */
template <typename T>
unsigned int Vector<T>::size() const {
    return _size;
}


/**
 * Checks whether the Vector is empty.
 * @return whether it's empty
 */
template <typename T>
bool Vector<T>::empty() const {
  return _size == 0;
}


/**
 * Increase the capacity to the new minimum capacity.
 * @param: min_cap -> The minimum capacity of the Vector
 */
template <typename T>
void Vector<T>::reserve(unsigned int min_cap) {
  if (_capacity >= min_cap) return;
  if (data == 0) {
    data = new T[min_cap];
  } else {
    T *old_data = data;
    data = new T[min_cap];
    for (unsigned int i = 0; i < _size; ++i) {
      data[i] = old_data[i];
    }
    delete [] old_data;
  }
  _capacity = min_cap;
}


/**
 * Increase the size to the new minimum size with the given fill item.
 * @param: min_size -> The minimum size of the Vector
 * @param: k        -> The fill character for any new space
 */
template <typename T>
void Vector<T>::resize(unsigned int min_size, T k) {
  if (min_size > _capacity) {
    reserve(min_size + 5);
  }
  if (min_size > _size) {
    for (unsigned int i = _size; i < min_size; ++i) {
      data[i] = k;
    }
  }
  _size = min_size;
}


/**
 * Get a reference to the first item in the Vector
 * @return a reference to the first item
 */
template <typename T>
T& Vector<T>::front() const {
  return data[0];
}


/**
 * Get a reference to the last item in the Vector
 * @return a reference to the last item
 */
template <typename T>
T& Vector<T>::back() const {
  return data[_size - 1];
}


/**
 * Remove the last item.
 */
template <typename T>
void Vector<T>::pop_back() {
  --_size;
}


/**
 * Add an item to the end
 * @param: x -> The item to be added
 */
template <typename T>
void Vector<T>::push_back(T x) {
  resize(_size + 1);
  data[_size - 1] = x;
}


/**
 * Get an iterator to the first item.
 * @return an iterator to the first item
 */
template <typename T>
typename Vector<T>::iterator Vector<T>::begin() const {
  return data;
}


/**
 * Get an iterator to the last item.
 * @return an iterator to the last item
 */
template <typename T>
typename Vector<T>::iterator Vector<T>::end() const {
  return data + _size;
}


#endif   // VECTOR_H
