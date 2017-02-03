//-----------------------------------------------------------------------------
// Copyright (c) 2013-2017 Benjamin Buch
//
// https://github.com/bebuch/BitmapViewer
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#define _USE_MATH_DEFINES
#include <cmath>

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

		/// \brief Brush with background texture for undef pixels
		QBrush make_background_brush(){
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

	colors::colors(palette_type t, unsigned strips):
		fold_(1),
		strips_(strips),
		min_(0),
		max_(255),
		auto_range_(true),
		int_range_(false),
		type_(t),
		contrast_type_(contrast_line_type::auto_color)
		{}


	void colors::contrast_line(bool enable){
		contrast_line_ = enable;
		update();
	}

	std::uint16_t colors::fold()const{
		return fold_;
	}

	unsigned colors::strips()const{
		return strips_;
	}

	double colors::min()const{
		return min_;
	}

	double colors::max()const{
		return max_;
	}

	bool colors::auto_range()const{
		return auto_range_;
	}

	bool colors::int_range()const{
		return int_range_;
	}



	void colors::set_fold(std::uint16_t fold){
		fold = fold ? fold : 1;
		if(fold_ != fold){
			fold_ = fold;
			update();
		}
	}

	void colors::set_strips(unsigned strips){
		strips_ = strips;
	}

	void colors::set_min(double pos){
		min_ = pos;
	}

	void colors::set_max(double pos){
		max_ = pos;
	}

	void colors::set_auto_range(bool on){
		auto_range_ = on;
	}

	void colors::set_int_range(bool on){
		int_range_ = on;
	}

	void colors::next_palette(){
		switch(type_){
			case palette_type::rainbow:{
				type_ = palette_type::gray;
				break;
			}
			case palette_type::gray:{
				type_ = palette_type::rainbow;
				break;
			}
		}
		update();
	}

	unsigned colors::step_pos(unsigned pos)const{
		return static_cast< unsigned >(
			pos / (unsigned_value_count / strips_)
		);
	}

	unsigned colors::pass_step_pos(unsigned step_pos)const{
		return static_cast< unsigned >(
			std::ceil(step_pos * (unsigned_value_count / strips_))
		);
	}

	unsigned colors::pass_pos(unsigned pos)const{
		return pass_step_pos(step_pos(pos));
	}

	void colors::set_contrast_type(contrast_line_type c){
		contrast_type_ = c;
	}


	std::vector< std::tuple< QString, colors::contrast_line_type > >
	colors::contrast_lines(){
		return {
			std::make_tuple(
				QString(QObject::tr("Color")),
				contrast_line_type::auto_color
			),
			std::make_tuple(
				QString(QObject::tr("Palette")),
				contrast_line_type::auto_palette
			)
		};
	}

	QColor colors::operator()(unsigned pos)const{
		bool const contrast = pass_pos(pos) <
			static_cast< unsigned >(unsigned_value_count / strips_);

		switch(type_){
			case palette_type::rainbow:{
				if(contrast_line_ && contrast){
					switch (contrast_type_){
					case contrast_line_type::auto_color:
						return QColor(0, 0, 0);
					case contrast_line_type::auto_palette:
						return calc_gray(pass_pos(pos * strips_));
					}
				}
				return calc_rainbow(pass_pos(pos) * fold_);
			}
			case palette_type::gray:{
				if(contrast_line_ && contrast){
					switch (contrast_type_){
					case contrast_line_type::auto_color:
						return QColor(255, 0, 0);
					case contrast_line_type::auto_palette:
						return calc_rainbow(pass_pos(pos * strips_));
					}
				}
				return calc_gray(pass_pos(pos) * fold_);
			}
		}
		return QColor(0, 0, 0);
	}

	QBrush const& colors::background_brush()const{
		static auto const brush = make_background_brush();
		return brush;
	}


}
