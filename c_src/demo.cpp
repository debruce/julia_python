#include <boost/python.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

namespace py = boost::python;
// using boost::noncopyable;

using namespace std;

char const* greet()
{
   return "hello, world";
}

string add_str(const string& a, const string b)
{
    cout << __PRETTY_FUNCTION__ << endl;
    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    return "plus";
}

vector<string> add_vstr(const vector<string>& a, const vector<string>& b)
{
    vector<string> ret{"julia", "robert"};

    cout << __PRETTY_FUNCTION__ << endl;
    for (const auto& item : a) {
        cout << "a = " << item << endl;
    }
    for (const auto& item : b) {
        cout << "b = " << item << endl;
    }
    return ret;
}

class MyObject : public boost::noncopyable {
    string  alpha;
    string  beta;

public:
    MyObject(const string& alpha_, const string& beta_) : alpha(alpha_), beta(beta_)
    {
        cout << __PRETTY_FUNCTION__ << " alpha=" << alpha << " beta=" << beta << endl;
    }

    ~MyObject()
    {
        cout << __PRETTY_FUNCTION__ << " alpha=" << alpha << " beta=" << beta << endl;
    }

    string get_alpha() { return alpha; }
    void set_alpha(const string& x) { alpha = x; }

    string get_beta() { return beta; }
    void set_beta(const string& x) { beta = x; }

    string __repr__()
    {
        stringstream ss;
        ss << "MyObject(" << alpha << ", " << beta << ")";
        return ss.str();
    }

    static void py_init()
    {
        using namespace py;
        using py::arg;

        class_<MyObject, boost::noncopyable, shared_ptr<MyObject>>("MyObject", init<string, string>(
                                                                                    (
                                                                                        arg("alpha"),
                                                                                        arg("beta")
                                                                                    )))
            .def("get_alpha", &MyObject::get_alpha)
            .def("set_alpha", &MyObject::set_alpha)
            .def("get_beta", &MyObject::get_beta)
            .def("get_beta", &MyObject::set_beta)
            .def("__repr__", &MyObject::__repr__)
            // .add_property("current", &RxCursor::get)
            // .add_property("max", &RxCursor::get_max)
            // .def("advance", &RxCursor::advance, arg("distance") = 1)
            // .def("next_record", &RxCursor::next_record)
            // .def("prev_record", &RxCursor::prev_record)
            // .def("next_change", &RxCursor::next_change, arg("ident") = py::object())
            // .def("prev_change", &RxCursor::prev_change, arg("ident") = py::object())
            // .def("__getattr__", &RxCursor::__getattr__)
            // .def("hasattr", &RxCursor::hasattr)
            // .def("attr_keys", &RxCursor::attr_keys)
            // .def("attr_items", &RxCursor::attr_items)
            // .add_property("extracted", &RxCursor::get_extracted)
            // .add_property("packet", &RxCursor::get_packet)
            ;
        }
};

template<typename T>
struct VectorToPythonConverter {
    static PyObject* convert(const std::vector<T>& vec)
    {
      py::list ret;
      for (auto iter = vec.cbegin(); iter != vec.cend(); iter++) ret.append(*iter);
      return py::incref(ret.ptr());
    }
};

template<typename T>
struct PythonToVectorConverter {
    PythonToVectorConverter()
    {
        py::converter::registry::push_back(&convertible, &construct, py::type_id<std::vector<T> >());
    }

    static void* convertible(PyObject* obj)
    {
        if (!PyList_Check(obj)) return 0;
        return obj;
    }

    static void construct(PyObject* obj, py::converter::rvalue_from_python_stage1_data* data)
    {
        py::list lst(py::borrowed(obj));
        void* storage = ((py::converter::rvalue_from_python_storage<std::vector<T> >*) data)->storage.bytes;
        auto v = new (storage) std::vector<T>(py::stl_input_iterator<T>(lst), py::stl_input_iterator<T>()); 
        data->convertible = storage;
    }
};

template<typename T>
struct py_vector {
    py::to_python_converter<std::vector<T>, VectorToPythonConverter<T> > toPy;
    PythonToVectorConverter<T> fromPy;
};

BOOST_PYTHON_MODULE(julia_demo)
{
    using namespace boost::python;

    def("greet", greet);
    def("add_str", add_str);
    def("add_vstr", add_vstr);

    MyObject::py_init();

    py_vector<std::string>();
    py_vector<int>();
}