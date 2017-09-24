// This project is a C++ version of qart4j,for more information see https://github.com/dieforfree/qart4j
// 
// Copyright (C) 2017 Swanny Peng <ph1994wh@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.



#pragma once
#ifndef GENERICGF_H
#define GENERICGF_H

#include<iostream>   
#include<cassert>  
#include <vector>
#include <string>
namespace qrgen {

	/**
	* <p>This class contains utility methods for performing mathematical operations over
	* the Galois Fields. Operations use a given primitive polynomial in calculations.</p>
	*
	* <p>Throughout this package, elements of the GF are represented as an {@code int}
	* for convenience and speed (but at the cost of memory).
	* </p>
	*
	* @author Sean Owen
	* @author David Olivier
	*
	* A java version rewrote by C++
	*/

	class GenericGF final {

	private:
		 std::vector<int> expTab;
	     std::vector<int> logTab;
	     int size;
		 int primitive;
	     int geneBase;
		
		/* Typical encoding generator */
	public:	
	/*
	* Create a representation of GF(size) using the given primitive polynomial.
	*
	*primitive	irreducible polynomial whose coefficients are represented by the bits of an int, where the least-significant bit represents the constant coefficient
	*size		the size of the field
	*b			the factor b in the generator polynomial can be 0- or 1-based (g(x) = (x+a^b)(x+a^(b+1))...(x+a^(b+2t-1))). In most cases it should be 1, but for QR code it is 0.
	*/
	public:
		GenericGF() { size = 0, primitive = 0,geneBase=0; }
		GenericGF(int primitive, int size, int b);
		~GenericGF() {
			expTab.clear(); expTab.swap(std::vector<int>());  logTab.clear(); logTab.swap(std::vector<int>());
		};
		 /*return sum/difference of a and b*/
		static int add_or_sub(int a, int b) {return a^b;}

		 /*2 to the power of a in GF(size)*/
		int exp(const int a) { return expTab[a]; }

		 /*base 2 log of a in GF(size)*/
		int log(const int a) { assert(!(a == 0)&&"Illegal Argument Exception!!!"); return logTab[a]; }

		 /*multiplicative inverse of a*/
		int inverse(const int a) { assert(!(a == 0)&& "Arithmetic Exception!!"); return expTab[size - 1 - logTab[a]]; }

		 /*product of a an d b in GF(size)*/
		int multiply(const int a, const int b){ return (a == 0 || b == 0) ? 0 : expTab[(logTab[a] + logTab[b]) % (size - 1)]; }

	    int getSize(){ return size; }

		int getGeneBase(){ return geneBase; }

		std::string to_String();
	};

	
}

#endif // !GENERICGF_H
