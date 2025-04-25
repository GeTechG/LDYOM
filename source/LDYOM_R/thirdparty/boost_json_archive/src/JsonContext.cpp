#include "boost/JsonContext.hpp"

#ifdef __GNUG__
#include <cstdlib>
#include <cxxabi.h>

struct handle {
  char *p;
  handle(char *ptr) : p(ptr) {}
  ~handle() { std::free(p); }
};

#endif

#ifdef __GNUG__

std::string demangle(const char *name) {
  int status = -4; // some arbitrary value to eliminate the compiler warning
  handle result(abi::__cxa_demangle(name, NULL, NULL, &status));
  return (status == 0) ? result.p : name;
}

#else
// does nothing if not g++
std::string demangle(const char *name) { return name; }
#endif

JsonContext::JsonContext(std::shared_ptr<boost::json::value> root) : m_context(), m_root{root}, m_current({"root", m_root}) {}

JsonContext::JsonContext() : m_context(), m_root(std::make_shared<boost::json::value>()), m_current({"root", m_root}) {}

bool JsonContext::empty() const { return m_context.empty(); }

void JsonContext::push(std::string tag, std::shared_ptr<boost::json::value> value, bool setCurrent) {
  m_context.push({tag, value});
  if (setCurrent) {
    m_current = top();
    m_current_tag = m_current.first;
  }
}

void JsonContext::pop() {
  m_context.pop();
  m_current = top();
}

JsonContext::stack_value_type &JsonContext::top() { return m_context.top(); }

const std::shared_ptr<boost::json::value> &JsonContext::root() { return m_root; }

void JsonContext::setRoot(std::string tag, std::shared_ptr<boost::json::value> v) {
  while (!m_context.empty())
    m_context.pop();
  m_root = v;
  m_current = {tag, m_root};
  m_current_tag = tag;
  m_context.push({tag, m_root});
}

JsonContext::stack_value_type &JsonContext::current() { return m_current; }

void JsonContext::setCurrent(std::string tag, std::shared_ptr<boost::json::value> &v) {
  m_current = {tag, v};
  m_current_tag = tag;
}

void JsonContext::setCurrentTag(std::string tag) { m_current_tag = tag; }

std::string JsonContext::currentTag() { return m_current_tag; }

void pretty_print(std::ostream &os, boost::json::value const &jv, std::string *indent = nullptr) {
  std::string indent_;
  if (!indent)
    indent = &indent_;
  switch (jv.kind()) {
  case boost::json::kind::object: {
    os << "{\n";
    indent->append(4, ' ');
    auto const &obj = jv.get_object();
    if (!obj.empty()) {
      auto it = obj.begin();
      for (;;) {
        os << *indent << boost::json::serialize(it->key()) << " : ";
        pretty_print(os, it->value(), indent);
        if (++it == obj.end())
          break;
        os << ",\n";
      }
    }
    os << "\n";
    indent->resize(indent->size() - 4);
    os << *indent << "}";
    break;
  }

  case boost::json::kind::array: {
    os << "[\n";
    indent->append(4, ' ');
    auto const &arr = jv.get_array();
    if (!arr.empty()) {
      auto it = arr.begin();
      for (;;) {
        os << *indent;
        pretty_print(os, *it, indent);
        if (++it == arr.end())
          break;
        os << ",\n";
      }
    }
    os << "\n";
    indent->resize(indent->size() - 4);
    os << *indent << "]";
    break;
  }

  case boost::json::kind::string: {
    os << boost::json::serialize(jv.get_string());
    break;
  }

  case boost::json::kind::uint64:
    os << jv.get_uint64();
    break;

  case boost::json::kind::int64:
    os << jv.get_int64();
    break;

  case boost::json::kind::double_:
    os << jv.get_double();
    break;

  case boost::json::kind::bool_:
    if (jv.get_bool())
      os << "true";
    else
      os << "false";
    break;

  case boost::json::kind::null:
    os << "null";
    break;
  }

  if (indent->empty())
    os << "\n";
}

void JsonContext::serialize(std::ostream &os, bool prettify) {
  if (prettify) {
    pretty_print(os, *m_root);
  } else {
    os << *m_root;
  }
}