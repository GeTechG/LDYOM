// Boost
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

// Boost Archive JSON
#include "boost/archive/json_iarchive.hpp"

#include <iostream>

namespace boost {
namespace archive {

json_iarchive::json_iarchive(std::istream &is, unsigned int) : m_ctx() {
  boost::system::error_code ec;
  root_value = JsonContext::parse(is, ec);
  if (ec) {
    throw std::runtime_error("Input stream is not Json Friendly...");
  }
}

void json_iarchive::load_override(class_name_type &t) {
  boost::json::value &data = m_ctx.current().second->get_object()[param::ClassNameType];

  if (!data.is_string()) {
    return;
  }

  std::strcpy((char *)t, data.as_string().c_str());
}

void json_iarchive::load_override(version_type &t) {
  boost::json::value &data = m_ctx.current().second->get_object()[param::VersionType];
  t = version_type(static_cast<uint64_t>(data.as_int64()));
}

void json_iarchive::load_override(object_id_type &t) {
  boost::json::value &data = m_ctx.current().second->get_object()[param::ObjectIdType];
  if (!data.is_number()) {
    object_reference_type r(object_id_type(0));
    load_override(r);
    t = object_id_type(r);
    return;
  }

  t = object_id_type(static_cast<uint64_t>(data.as_int64()));
}

void json_iarchive::load_override(object_reference_type &t) {
  boost::json::value &data = m_ctx.current().second->get_object()[param::ObjectReferenceType];
  if (!data.is_number()) {
    return;
  }

  t = object_reference_type(object_id_type(data.as_int64()));
}

void json_iarchive::load_override(class_id_type &t) {
  if (m_ctx.current().second->kind() != json::kind::object) {
    m_ctx.pop();
  }
  boost::json::value &data = m_ctx.current().second->get_object()[param::ClassIdType];
  if (!data.is_number()) {
    class_id_reference_type r(class_id_type(0));
    load_override(r);
    t = class_id_type(int(r));
    return;
  }

  t = class_id_type(static_cast<size_t>(data.as_int64()));
}

void json_iarchive::load_override(class_id_optional_type &t) {
  if (m_ctx.current().second->kind() != json::kind::object) {
    m_ctx.pop();
  }
  boost::json::value &data = m_ctx.current().second->get_object()[param::ClassIdOptionalType];

  if (!data.is_number()) {
    return;
  }

  t = class_id_optional_type(class_id_type(static_cast<size_t>(data.as_int64())));
}

void json_iarchive::load_override(class_id_reference_type &t) {
  if (m_ctx.current().second->kind() != json::kind::object) {
    m_ctx.pop();
  }
  boost::json::value &data = m_ctx.current().second->get_object()[param::ClassIdReferenceType];
  if (!data.is_number()) {
    return;
  }
  t = class_id_reference_type(class_id_type(static_cast<size_t>(data.as_int64())));
}

void json_iarchive::load_override(tracking_type &t) {
  if (m_ctx.current().second->kind() != json::kind::object) {
    m_ctx.pop();
  }
  boost::json::value &data = m_ctx.current().second->get_object()[param::TrackingType];

  if (!data.is_bool()) {
    return;
  }
  t = tracking_type(data.as_bool());
}

template class detail::archive_serializer_map<json_iarchive>;

} // namespace archive
} // namespace boost
