#ifndef MACROS_HPP
#define MACROS_HPP

#define HAS_GETTER(ret_type__, getter_name__) const ret_type__ & getter_name__ () const noexcept

#define NOT_COPYABLE(klazz__) klazz__ (const  klazz__ &) = delete; void operator=(const klazz__ &) = delete

#define GETTER(ret_type__, name__, field__) const ret_type__ & name__ () const noexcept { return field__; }

#endif