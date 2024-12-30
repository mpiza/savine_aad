# Python Bindings for Savine AAD Library

This directory contains Python bindings for Antoine Savine's Automatic Algorithmic Differentiation (AAD) library, using pybind11.

## Requirements

- CMake (>= 3.10)
- Python (>= 3.6)
- C++ compiler with C++17 support
- pybind11 (automatically downloaded during build)

## Building the Bindings

1. Create a build directory:
```bash
cd python_bindings
mkdir build
cd build
```

2. Configure and build:
```bash
cmake ..
cmake --build . --config Release
```

This will create a Python module named `savine_aad` in the python_bindings directory.

## Usage

After building, you can use the library in Python. Here's a simple example:

```python
import savine_aad as aad

# Create AAD numbers
x = aad.Number(2.0)
y = aad.Number(3.0)

# Put them on tape for derivative tracking
x.put_on_tape()
y.put_on_tape()

# Perform calculations
z = x * y + x * x

# Calculate derivatives
z.propagate_to_start()

# Get results
print(f"z = {float(z)}")
print(f"dz/dx = {x.adjoint()}")
print(f"dz/dy = {y.adjoint()}")
```

You can also use the built-in Black-Scholes calculator:

```python
result = aad.example_black_scholes(
    spot=100.0,    # Spot price
    strike=100.0,  # Strike price
    vol=0.2,       # Volatility
    r=0.05,        # Risk-free rate
    T=1.0          # Time to maturity
)

print(f"Price: {result['price']}")
print(f"Delta: {result['delta']}")
print(f"Vega:  {result['vega']}")
print(f"Rho:   {result['rho']}")
```

## Features

Currently implemented features:
- Basic AAD Number class with arithmetic operations
- Tape management for derivative tracking
- Example Black-Scholes calculator with Greeks computation

## Running Tests

You can run the example script to test the bindings:

```bash
python example.py
``` 