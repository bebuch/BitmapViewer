//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <BitmapViewer/colors.hpp>

#include <QPainter>


namespace bitmap_viewer{


	namespace{


		constexpr double unsigned_value_count =
			std::numeric_limits< unsigned >::max() + 1.0;

		QColor calc_rainbow(unsigned i){
			double pos = i / unsigned_value_count;

			if(pos < 1. / 6){
				pos *= 3 * M_PI;
				return QColor(255, 0, std::sin(M_PI / 2 - pos) * 255);
			}else if(pos >= 1. / 6 && pos < 2. / 6){
				pos = (pos - 1. / 6) * 3 * M_PI;
				return QColor(255, std::sin(pos) * 255, 0);
			}else if(pos >= 2. / 6 && pos < 3. / 6){
				pos = (pos - 2. / 6) * 3 * M_PI;
				return QColor(std::sin(M_PI / 2 - pos) * 255, 255, 0);
			}else if(pos >= 3. / 6 && pos < 4. / 6){
				pos = (pos - 3. / 6) * 3 * M_PI;
				return QColor(0, 255, std::sin(pos) * 255);
			}else if(pos >= 4. / 6 && pos < 5. / 6){
				pos = (pos - 4. / 6) * 3 * M_PI;
				return QColor(0, std::sin(M_PI / 2 - pos) * 255, 255);
			}else{
				pos = (pos - 5. / 6) * 3 * M_PI;
				return QColor(std::sin(pos) * 255, 0, 255);
			}
		}

		QColor calc_gray(unsigned i){
			int const c = static_cast< int >(i / unsigned_value_count * 256);
			return QColor(c, c, c);
		}

		QBrush make_brush(){
			QBrush brush;
			QPixmap background(16, 16);
			{
				QPainter painter(&background);
				painter.setPen(Qt::NoPen);
				painter.setBrush(QBrush(0x999999));
				painter.drawRect(0, 0, 8, 8);
				painter.drawRect(8, 8, 8, 8);
				painter.setBrush(QBrush(0x666666));
				painter.drawRect(8, 0, 8, 8);
				painter.drawRect(0, 8, 8, 8);
			}
			brush.setTexture(background);
			return brush;
		}


	}

	colors::colors(type t):
		fold_(1),
		type_(t)
		{}


	void colors::contrast_line(bool enable){
		contrast_line_ = enable;
		update();
	}

	std::uint16_t colors::fold()const{
		return fold_;
	}

	void colors::set_fold(std::uint16_t fold){
		fold = fold ? fold : 1;
		if(fold_ != fold){
			fold_ = fold;
			update();
		}
	}

	void colors::next_palette(){
		switch(type_){
			case type::rainbow:{
				type_ = type::gray;
				break;
			}
			case type::gray:{
				type_ = type::rainbow;
				break;
			}
		}
		update();
	}

	QColor colors::operator()(unsigned i)const{
		bool const contrast = i < (unsigned_value_count / 100);

		switch(type_){
			case type::rainbow:{
				if(contrast_line_ && contrast) return calc_gray(i * 100);
				return calc_rainbow(i * fold_);
			}
			case type::gray:{
				if(contrast_line_ && contrast) return calc_rainbow(i * 100);
				return calc_gray(i * fold_);
			}
		}
		return QColor();
	}

	QBrush const& colors::brush()const{
		static auto brush = make_brush();
		return brush;
	}


}
