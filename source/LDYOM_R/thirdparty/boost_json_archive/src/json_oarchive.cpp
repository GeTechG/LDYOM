// C++ Standard Library
#include <algorithm>
#include <iterator>

// Boost
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/impl/archive_serializer_map.ipp>

// Boost Archive JSON
#include "boost/archive/json_oarchive.hpp"

namespace boost {
namespace archive {

json_oarchive::json_oarchive(std::ostream &os, unsigned int, const bool prettify) : m_ctx{}, os_{os}, prettify_{prettify} {}

json_oarchive::~json_oarchive() { m_ctx.serialize(os_, prettify_); }

void json_oarchive::save_override(const class_name_type &t) { m_ctx.current().second->as_object()[param::ClassNameType] = t.t; }

void json_oarchive::save_override(const version_type &t) { m_ctx.current().second->as_object()[param::VersionType] = t; }

void json_oarchive::save_override(const object_id_type &t) { m_ctx.current().second->as_object()[param::ObjectIdType] = t; }

void json_oarchive::save_override(const object_reference_type &t) {
  m_ctx.current().second->as_object()[param::ObjectReferenceType] = t;
}

void json_oarchive::save_override(const class_id_type &t) { m_ctx.current().second->as_object()[param::ClassIdType] = t; }

void json_oarchive::save_override(const class_id_optional_type &t) {
  m_ctx.current().second->as_object()[param::ClassIdOptionalType] = t;
}

void json_oarchive::save_override(const class_id_reference_type &t) {
  m_ctx.current().second->as_object()[param::ClassIdReferenceType] = t;
}

void json_oarchive::save_override(const tracking_type &t) { m_ctx.current().second->as_object()[param::TrackingType] = t.t; }

template class detail::archive_serializer_map<json_oarchive>;

} // namespace archive
} // namespace boost
