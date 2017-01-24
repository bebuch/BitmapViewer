//-----------------------------------------------------------------------------
// Copyright (c) 2009-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _bitmap_viewer__point__hpp_INCLUDED_
#define _bitmap_viewer__point__hpp_INCLUDED_


namespace bitmap_viewer{

	/// \brief A class for manipulating points
	/// \tparam ValueType Type of the position data
	template < typename ValueType >
	class point{
	public:
		/// \brief Type of the positions
		using value_type = ValueType;


		/// \brief The x
		value_type& x(){
			return x_;
		}

		/// \brief The y
		value_type& y(){
			return y_;
		}


		/// \brief The x
		value_type const& x()const{
			return x_;
		}

		/// \brief The y
		value_type const& y()const{
			return y_;
		}


		/// \brief Set x and y
		void set(value_type const& x, value_type const& y){
			x_ = x;
			y_ = y;
		}


	#if !defined(_MSC_VER) || _MSC_VER > 1800
		/// \brief Constructs a point by (0, 0)
		point() = default;

		/// \brief Copy constructor
		point(point const&) = default;

		/// \brief Move constructor
		point(point&&) = default;
	#else
		point(): x_(), y_() {}
	#endif

		/// \brief Constructs a point by (x, y)
		point(value_type const& x, value_type const& y):
			x_(x), y_(y)
			{}


	#if !defined(_MSC_VER) || _MSC_VER > 1800
		/// \brief Copy assignment
		point& operator=(point const&) = default;

		/// \brief Move assignment
		point& operator=(point&&) = default;
	#endif


		/// \brief Get true, if width and height are positiv
		bool is_positive()const{
			return x() >= value_type() && y() >= value_type();
		}


	private:
		value_type x_;
		value_type y_;
	};


}

#endif
