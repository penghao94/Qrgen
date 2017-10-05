// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma once
#ifndef GENERICGFPOLY_H
#define GENERICGFPOLY_H
#include "GenericGF.h"
#include<vector>
#include<string>
namespace qrgen {
	/**
	* <p>Represents a polynomial whose coefficients are elements of a GF.
	* Instances of this class are immutable.</p>
	*
	* <p>Much credit is due to William Rucklidge since portions of this code are an indirect
	* port of his C++ Reed-Solomon implementation.</p>
	*
	* @author Sean Owen
	*
	* A java version rewrote by C++
	*/
	class  GenericGFPoly final {
	public:
		GenericGF *field;
		std::vector<int> coeff;
		
	public:
		/**
		*field			the {@link GenericGF} instance representing the field to use to perform computations
		*coefficients	coefficients as ints representing elements of GF(size), arranged from most significant (highest-power term) coefficient to least significant
		*
		*throws IllegalArgumentException if argument is null or empty,
		* or if leading coefficient is 0 and this is not a
		* constant polynomial (that is, it is not the monomial "0")
		*/
		GenericGFPoly(qrgen::GenericGF *field, std::vector<int> &coef);
		~GenericGFPoly() { coeff.clear(); coeff.swap(std::vector<int>()); }

		GenericGFPoly * getZero();

		GenericGFPoly * getOne(); 

		/*return the monomial representing coefficient * x^degree*/
		//qrgen::GenericGFPoly *buildMonomial(int d, int c);

		/* return coefficients of this polynomial */
		std::vector<int> getCoeff() { return coeff; }

		/*return degree of of this polynomial */
		int getDegree() { return static_cast<int>(coeff.size())-1; }
		
		/*return if this polynomial is the monomial "0"*/
		bool isZero() { return coeff[0] == 0; }

		/* return coefficient of x^degree term in this polynomial*/
		int getCoeff(const int degree) { return coeff[coeff.size() - 1 - degree]; }

		/* return evaluation of this polynomial at given point*/
		int evaluateAt(const int a);

		/*return sum/difference of field and other*/
		GenericGFPoly* add_or_sub(GenericGFPoly* other);

		/*return product of field and scalar*/
		GenericGFPoly* multiply(const int scalar);

		GenericGFPoly* multiply(GenericGFPoly *other);

		/*return product of monomial in degree d and coefficient c*/
		GenericGFPoly* multiply_by_monomial(const int d, const int c);
		
		/*return quotient and reminder divided by polynomial other */
		std::vector<GenericGFPoly*> divide(GenericGFPoly *other);

		GenericGFPoly * buildMonomial(int d, int c);
		/* return string*/
		std::string to_string();
	};

	

}
#endif // !GENERICGFPOLY_H

