#include <Python.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>


struct Empty
{
  Empty()
  {
    std::cout << "creating Empty instance at " << this << std::endl;
  }
  ~Empty() {
    std::cout << "Deleting Empty instance at: " << this << std::endl;
  }
};

struct Store
{
    std::weak_ptr<Empty> value;
    Store() {
      std::cout << "creating Store instance at " << this << std::endl;
    }
    ~Store() {
      std::cout << "Deleting Store instance at: " << this << std::endl;
    }
    void setValue(std::shared_ptr<Empty> v) {
        std::cout << "storing " << v << std::endl;
        this->value = std::weak_ptr<Empty>(v);
        std::shared_ptr<Empty> v_ok = this->value.lock();
        if (v_ok) {
            std::cout << "ok, v has been stored" << std::endl;
        }
    }

    std::shared_ptr<Empty> getValue() {
        std::shared_ptr<Empty> p = this->value.lock();
        if (p) {
            std::cout << "stored value : " << p << std::endl;
        } else {
            std::cout << "there's nothing here !" << std::endl;
        }
        return p;
    }
};

template <class T>
void deleteShPtr(PyObject* capsule)
{
  if (!PyCapsule_IsValid(capsule, NULL)) {
    assert(false && "delete expects a valid capsule as an argument.");
  }
  void* pointer = PyCapsule_GetPointer(capsule, NULL);
  std::shared_ptr<T>* object = (std::shared_ptr<T>*)pointer;
  delete object;
}

template <class T>
PyObject* createInstance(PyObject* /*self*/, PyObject* args)
{
  if (!PyArg_ParseTuple(args, ""))
    return NULL;
  // Create a pointer to a shared pointer to a new T instance
  std::shared_ptr<T>* empty = new std::shared_ptr<T>((new T()));
  return PyCapsule_New((void*)empty, NULL, deleteShPtr<T>);
}

PyObject* storeSetValue(PyObject* /*self*/, PyObject* args)
{
  PyObject* pyStore, *pyEmpty;
  void *pointer;
  if (!PyArg_ParseTuple(args, "OO", &pyStore, &pyEmpty))
    return NULL;
  if (!PyCapsule_IsValid(pyStore, NULL)) {
    PyErr_SetString(PyExc_TypeError,
		    "function takes a PyCObject as argument");
    return NULL;
  }
  if (!PyCapsule_IsValid(pyEmpty, NULL)) {
    PyErr_SetString(PyExc_TypeError,
		    "function takes a PyCObject as argument");
    return NULL;
  }
  pointer = PyCapsule_GetPointer(pyStore, NULL);
  std::shared_ptr<Store>* store = (std::shared_ptr<Store>*)pointer;
  pointer = PyCapsule_GetPointer(pyEmpty, NULL);
  std::shared_ptr<Empty>* empty = (std::shared_ptr<Empty>*)pointer;
  (*store)->setValue(*empty);
  return Py_BuildValue("");
}

PyObject* storeGetValue(PyObject* /*self*/, PyObject* args)
{
  PyObject* pyStore;
  void *pointer;
  if (!PyArg_ParseTuple(args, "O", &pyStore))
    return NULL;
  if (!PyCapsule_IsValid(pyStore, NULL)) {
    PyErr_SetString(PyExc_TypeError,
		    "function takes a PyCObject as argument");
    return NULL;
  }
  pointer = PyCapsule_GetPointer(pyStore, NULL);
  std::shared_ptr<Store>* store = (std::shared_ptr<Store>*)pointer;
  std::shared_ptr<Empty>* empty = new std::shared_ptr<Empty>((*store)->getValue());
  pointer = (void*)empty;
  return PyCapsule_New(pointer, NULL, deleteShPtr<Empty>);
}

static PyMethodDef testPythonMethods[] = {
  {"create_empty", createInstance<Empty>, METH_VARARGS, "create an instance of Empty."},
  {"create_store", createInstance<Store>, METH_VARARGS, "create an instance of Store."},
  {"store_setValue", storeSetValue, METH_VARARGS, "bind Store::setValue."},
  {"store_getValue", storeGetValue, METH_VARARGS, "bind Store::getValue."},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef testPythonModule = {
  PyModuleDef_HEAD_INIT,
  "test_python_cxx",
  "test of C/Python API",
  -1,
  testPythonMethods
};

PyMODINIT_FUNC
PyInit_test_python_cxx(void)
{
  return PyModule_Create(&testPythonModule);
}
