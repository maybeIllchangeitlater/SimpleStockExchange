#ifndef SIMPLESTOCKEXCHANGE_BINDER_HPP_
#define SIMPLESTOCKEXCHANGE_BINDER_HPP_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/enable_shared_from_this.hpp>

namespace s21{
    struct Binder{
        template<class Object, typename Func, typename ... Args>
        static auto BindTo(Object& obj, Func func, Args &&... args) {
            if constexpr (sizeof ... (Args) == 0){
                return boost::bind(func, obj.shared_from_this());
            }else {
                return boost::bind(func, obj.shared_from_this(), std::forward<Args>(args)...);
            }
        }
    };
}

#endif //SIMPLESTOCKEXCHANGE_BINDER_HPP_
