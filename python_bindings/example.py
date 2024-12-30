import savine_aad as aad

def test_black_scholes():
    # Example parameters
    spot = 100.0
    strike = 100.0
    vol = 0.2
    r = 0.05
    T = 1.0

    # Calculate price and Greeks
    result = aad.example_black_scholes(spot, strike, vol, r, T)
    
    print("Black-Scholes Option Pricing Results:")
    print(f"Price: {result['price']:.6f}")
    print(f"Delta: {result['delta']:.6f}")
    print(f"Vega:  {result['vega']:.6f}")
    print(f"Rho:   {result['rho']:.6f}")

def test_manual_aad():
    # Create some Numbers
    x = aad.Number(2.0)
    y = aad.Number(3.0)

    # Put them on tape
    x.put_on_tape()
    y.put_on_tape()

    # Do some calculations
    z = x * y + x * x

    # Calculate derivatives
    z.propagate_to_start()

    print("\nManual AAD Test:")
    print(f"z = x * y + x * x")
    print(f"z = {float(z):.6f}")
    print(f"dz/dx = {x.adjoint():.6f}")  # Should be y + 2x = 7
    print(f"dz/dy = {y.adjoint():.6f}")  # Should be x = 2

if __name__ == "__main__":
    test_black_scholes()
    test_manual_aad() 