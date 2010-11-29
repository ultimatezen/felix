#ifndef VECTORX_H
#define VECTORX_H

// VectorX.h -- high-level vector functions
// Author: Brandon Corfman, 7/23/02

#include <vector>
#include <math.h>

namespace easystl
{
	//
	// make_vector -- returns a vector containing the passed elements
	//
	template <typename T> std::vector<T> make_vector(const T& e1)
	{
		std::vector<T> v(1);
		v[0] = e1;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2)
	{
		std::vector<T> v(2);
		v[0] = e1; v[1] = e2;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2, const T& e3)
	{
		std::vector<T> v(3);
		v[0] = e1; v[1] = e2; v[2] = e3;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2, const T& e3, const T& e4)
	{
		std::vector<T> v(4);
		v[0] = e1; v[1] = e2; v[2] = e3; v[3] = e4;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5)
	{
		std::vector<T> v(5);
		v[0] = e1; v[1] = e2; v[2] = e3; v[3] = e4;	v[4] = e5;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
												const T& e6)
	{
		std::vector<T> v(6);
		v[0] = e1; v[1] = e2; v[2] = e3; v[3] = e4; v[4] = e5;
		v[5] = e6;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
												const T& e6, const T& e7)
	{
		std::vector<T> v(7);
		v[0] = e1; v[1] = e2; v[2] = e3; v[3] = e4; v[4] = e5;
		v[5] = e6; v[6] = e7;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
												const T& e6, const T& e7, const T& e8)
	{
		std::vector<T> v(8);
		v[0] = e1; v[1] = e2; v[2] = e3; v[3] = e4; v[4] = e5;
		v[5] = e6; v[6] = e7; v[7] = e8;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
												const T& e6, const T& e7, const T& e8, const T& e9)
	{
		std::vector<T> v(9);
		v[0] = e1; v[1] = e2; v[2] = e3; v[3] = e4; v[4] = e5;
		v[5] = e6; v[6] = e7; v[7] = e8; v[8] = e9;
		return v;
	}

	template <typename T> std::vector<T> make_vector(const T& e1, const T& e2, const T& e3, const T& e4, const T& e5,
												const T& e6, const T& e7, const T& e8, const T& e9, const T& e10)
	{
		std::vector<T> v(9);
		v[0] = e1; v[1] = e2; v[2] = e3; v[3] = e4; v[4] = e5;
		v[5] = e6; v[6] = e7; v[7] = e8; v[8] = e9; v[9] = e10;
		return v;
	}

	// 
	// zip -- combines corresponding items from two or more vectors and 
	//        returns them as a vector of vectors, stopping after it has
	//        processed all the items in the shortest vector.
	//        This set of functions can handle up to 10 vectors.
	//
	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = std::min(v1.size(), v2.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(2);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
		}
		return vv;
	}

	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2, 
			const std::vector<T>& v3)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = min(v1.size(), v2.size(), v3.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(3);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
			vv[i][2] = v3[i];
		}
		return vv;
	}

	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2, 
			const std::vector<T>& v3, const std::vector<T>& v4)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = min(v1.size(), v2.size(), v3.size(), v4.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(4);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
			vv[i][2] = v3[i];
			vv[i][3] = v4[i];
		}
		return vv;
	}

	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2, 
			const std::vector<T>& v3, const std::vector<T>& v4,
			const std::vector<T>& v5)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = min(v1.size(), v2.size(), v3.size(), v4.size(), v5.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(5);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
			vv[i][2] = v3[i];
			vv[i][3] = v4[i];
			vv[i][4] = v5[i];
		}
		return vv;
	}

	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2, 
			const std::vector<T>& v3, const std::vector<T>& v4,
			const std::vector<T>& v5, const std::vector<T>& v6)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = min(v1.size(), v2.size(), v3.size(), v4.size(), v5.size(),
			             v6.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(6);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
			vv[i][2] = v3[i];
			vv[i][3] = v4[i];
			vv[i][4] = v5[i];
			vv[i][5] = v6[i];
		}
		return vv;
	}

	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2, 
			const std::vector<T>& v3, const std::vector<T>& v4,
			const std::vector<T>& v5, const std::vector<T>& v6,
			const std::vector<T>& v7)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = min(v1.size(), v2.size(), v3.size(), v4.size(), v5.size(),
			             v6.size(), v7.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(7);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
			vv[i][2] = v3[i];
			vv[i][3] = v4[i];
			vv[i][4] = v5[i];
			vv[i][5] = v6[i];
			vv[i][6] = v7[i];
		}
		return vv;
	}

	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2, 
			const std::vector<T>& v3, const std::vector<T>& v4,
			const std::vector<T>& v5, const std::vector<T>& v6,
			const std::vector<T>& v7, const std::vector<T>& v8)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = min(v1.size(), v2.size(), v3.size(), v4.size(), v5.size(),
			             v6.size(), v7.size(), v8.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(8);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
			vv[i][2] = v3[i];
			vv[i][3] = v4[i];
			vv[i][4] = v5[i];
			vv[i][5] = v6[i];
			vv[i][6] = v7[i];
			vv[i][7] = v8[i];
		}
		return vv;
	}

	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2, 
			const std::vector<T>& v3, const std::vector<T>& v4,
			const std::vector<T>& v5, const std::vector<T>& v6,
			const std::vector<T>& v7, const std::vector<T>& v8,
			const std::vector<T>& v9)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = min(v1.size(), v2.size(), v3.size(), v4.size(), v5.size(),
			             v6.size(), v7.size(), v8.size(), v9.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(9);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
			vv[i][2] = v3[i];
			vv[i][3] = v4[i];
			vv[i][4] = v5[i];
			vv[i][5] = v6[i];
			vv[i][6] = v7[i];
			vv[i][7] = v8[i];
			vv[i][8] = v9[i];
		}
		return vv;
	}

	template <typename T> std::vector<std::vector<T> > 
		zip(const std::vector<T>& v1, const std::vector<T>& v2, 
			const std::vector<T>& v3, const std::vector<T>& v4,
			const std::vector<T>& v5, const std::vector<T>& v6,
			const std::vector<T>& v7, const std::vector<T>& v8,
			const std::vector<T>& v9, const std::vector<T>& v10)
	{
		typedef std::vector<std::vector<T> > Vec2D;
		typedef std::vector<T>::size_type VecSize;
		
		VecSize sz = min(v1.size(), v2.size(), v3.size(), v4.size(), v5.size(),
			             v6.size(), v7.size(), v8.size(), v9.size(), v10.size());
		Vec2D vv(sz);
		for (VecSize i=0; i<sz; i++)
		{
			vv[i].resize(10);
			vv[i][0] = v1[i];
			vv[i][1] = v2[i];
			vv[i][2] = v3[i];
			vv[i][3] = v4[i];
			vv[i][4] = v5[i];
			vv[i][5] = v6[i];
			vv[i][6] = v7[i];
			vv[i][7] = v8[i];
			vv[i][8] = v9[i];
			vv[i][9] = v10[i];
		}
		return vv;
	}

	// 
	// range -- returns a vector filled with values between supplied beginning 
	//          and ending values. An optional step size value can also be 
	//          provided.
	//
	template <typename T> std::vector<T> range(const T val1, const T val2,
											   const double stepsize = 1.0)
	{
		// trap invalid values
		if (val1 >= val2)
			return std::vector<T>();

		// cast stepsize to appropriate type
		T step = static_cast<T>(stepsize);

		// fill the vector
		T diff = abs(val2-val1)/step;
		std::vector<T> v(diff+1);
		T fill = val1;
		for (T i=0; i<=diff; i++)
		{
			v[i] = fill;
			fill += step;
		}
		return v;
	}

	// specialization for doubles
	inline std::vector<double> range(const double val1, const double val2,
									 const double step)
	{
		typedef std::vector<double> DblVec;
		typedef std::vector<double>::size_type DblVecSize;

		// trap invalid values
		if (val1 >= val2)
			return DblVec();

		// fill the vector
		DblVecSize diff = static_cast<DblVecSize>(fabs(val2-val1)/step);
	    DblVec v(diff+1);
		double fill = val1;
		for (DblVecSize i=0; i<=diff; i++)
		{
			v[i] = fill;
			fill += step;
		}
		return v;
	}

	// specialization for floats
	inline std::vector<float> range(const float val1, const float val2,
		    						const double stepsize)
	{
		typedef std::vector<float> FltVec;
		typedef std::vector<float>::size_type FltVecSize;

		// trap invalid values
		if (val1 >= val2)
			return FltVec();

		// cast stepsize to float
		float step = static_cast<float>(stepsize);

		// fill the vector
		FltVecSize diff = static_cast<FltVecSize>(fabs(val2-val1)/step);
	    FltVec v(diff+1);
		float fill = val1;
		for (FltVecSize i=0; i<=diff; i++)
		{
			v[i] = fill;
			fill += step;
		}
		return v;
	}
}

#endif
