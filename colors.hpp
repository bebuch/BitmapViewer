#ifndef _bitmap_viewer_colors_hpp_INCLUDED_
#define _bitmap_viewer_colors_hpp_INCLUDED_

#include <QColor>
#include <QBrush>

#include <boost/signals2.hpp>


namespace bitmap_viewer{


class colors{
public:
	enum class type { rainbow, gray };

	colors(type t = type::rainbow);

	boost::signals2::signal< void() > update;

	std::uint16_t fold()const;
	void set_fold(std::uint16_t fold);

	void contrast_line(bool enable);
	void next();

	QBrush const& brush()const;
	QColor operator()(unsigned i)const;

private:
	bool contrast_line_ = false;
	std::uint16_t fold_;
	type type_;
};


}


#endif
