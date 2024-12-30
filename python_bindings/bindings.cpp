#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include "AAD.h"
#include "mcBase.h"
#include "mcMdl.h"
#include "mcPrd.h"

namespace py = pybind11;

// Helper function to convert Python list to vector<Number>
std::vector<Number> py_list_to_numbers(const py::list& list) {
    std::vector<Number> result;
    for (const auto& item : list) {
        result.push_back(Number(item.cast<double>()));
    }
    return result;
}

// Helper function to convert vector<Number> to Python list
py::list numbers_to_py_list(const std::vector<Number>& numbers) {
    py::list result;
    for (const auto& num : numbers) {
        result.append(py::cast(double(num)));
    }
    return result;
}

PYBIND11_MODULE(savine_aad, m) {
    m.doc() = "Python bindings for Savine's AAD library"; // Module docstring

    // Expose Number class
    py::class_<Number>(m, "Number")
        .def(py::init<double>())
        .def("value", &Number::value)
        .def("adjoint", (double (Number::*)() const) &Number::adjoint)
        .def("put_on_tape", &Number::putOnTape)
        .def("propagate_to_start", &Number::propagateToStart)
        .def("__float__", [](const Number& n) { return double(n); })
        .def("__add__", [](const Number& a, const Number& b) { return a + b; })
        .def("__sub__", [](const Number& a, const Number& b) { return a - b; })
        .def("__mul__", [](const Number& a, const Number& b) { return a * b; })
        .def("__truediv__", [](const Number& a, const Number& b) { return a / b; })
        .def("__radd__", [](const Number& a, double b) { return Number(b) + a; })
        .def("__rsub__", [](const Number& a, double b) { return Number(b) - a; })
        .def("__rmul__", [](const Number& a, double b) { return Number(b) * a; })
        .def("__rtruediv__", [](const Number& a, double b) { return Number(b) / a; });

    // Expose Tape class
    py::class_<Tape>(m, "Tape")
        .def(py::init<>())
        .def("clear", &Tape::clear)
        .def("rewind", &Tape::rewind)
        .def("mark", &Tape::mark)
        .def("rewind_to_mark", &Tape::rewindToMark);

    // Expose global tape
    m.attr("global_tape") = &globalTape;

    // Example function to demonstrate AAD
    m.def("example_black_scholes", [](double spot, double strike, double vol, double r, double T) {
        // Put parameters on tape
        Number S(spot);
        Number K(strike);
        Number sigma(vol);
        Number rf(r);
        Number expiry(T);

        S.putOnTape();
        sigma.putOnTape();
        rf.putOnTape();

        // Black-Scholes formula
        Number d1 = (log(S/K) + (rf + sigma*sigma*0.5)*expiry) / (sigma*sqrt(expiry));
        Number d2 = d1 - sigma*sqrt(expiry);

        Number price = S * normalCdf(d1) - K * exp(-rf*expiry) * normalCdf(d2);

        // Calculate sensitivities
        price.propagateToStart();

        // Return results as a dictionary
        py::dict result;
        result["price"] = double(price);
        result["delta"] = S.adjoint();
        result["vega"] = sigma.adjoint();
        result["rho"] = rf.adjoint();

        return result;
    }, "Calculate Black-Scholes price and Greeks using AAD",
    py::arg("spot"), py::arg("strike"), py::arg("vol"), py::arg("r"), py::arg("T"));
} 