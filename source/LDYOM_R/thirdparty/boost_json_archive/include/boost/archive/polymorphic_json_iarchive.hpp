#ifndef BOOST_ARCHIVE_POLYMORPHIC_JSON_IARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_JSON_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// polymorphic_json_iarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include "json_iarchive.hpp"
#include <boost/archive/detail/polymorphic_iarchive_route.hpp>

typedef boost::archive::detail::polymorphic_iarchive_route<boost::archive::json_iarchive> polymorphic_json_iarchive;

#include <boost/version.hpp>
#if BOOST_VERSION > 103401
// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(polymorphic_json_iarchive)
#endif

#endif // BOOST_ARCHIVE_POLYMORPHIC_JSON_OARCHIVE_HPP