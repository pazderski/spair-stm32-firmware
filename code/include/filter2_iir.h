#pragma once
#include <math.h>

// filtr o transmitancji: G(z^-1)=(b0+b1 z^-1+b2 z^-2)/(1+a1 z^-1+a2 z^-2)
class Filter2Iir
{

private:

	float a[3];
	float b[3];
	float u[3];
	float y[3];

public:

	float output;

	Filter2Iir(float * p)
	{
		for(size_t i = 0; i < 3; i++)
		{
			a[i] = p[i];
			b[i] = p[i+3];
		}
		u[0] = u[1] = u[2] = 0.0;
		y[0] = y[1] = y[2] = 0.0;
	}

	float CalculateOutput(float input)
	{

		float out = 0;
		u[0] = input;
		// wyznaczenie odpowiedzi filtru
		for(size_t i = 0; i < 3; i++)
		{
			out += b[i] * u[i];
		}
		for(size_t i = 1; i < 3; i++)
		{
			out -= a[i] * y[i];
		}

		y[0] = out;
		// przepisanie probek w buforach filtru
		for(size_t i = 2; i > 0; i--)
		{
			u[i] = u[i-1];
			y[i] = y[i-1];
		}

		output = out;
		return out;
	}

	void SetSteadyState(float input)
	{
		u[0] = u[1] = u[2] = input;
		y[0] = y[1] = y[2] = input;
		output = input;

	}
};
