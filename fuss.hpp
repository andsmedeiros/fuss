#ifndef FUSS_HPP
#define FUSS_HPP

#include <list>
#include <functional>
#include <mutex>

namespace fuss {
  
template<class ... T_args>
struct message {
    using handler = std::function<void(T_args ...)>;
    using listener = typename std::list<handler>::iterator;
};

template<class T_message, class ... T_rest>
struct shouter : public shouter<T_message>, public shouter<T_rest ...> {
  using shouter<T_message>::listen;
  using shouter<T_message>::unlisten;
  using shouter<T_message>::shout;
  using shouter<T_rest ...>::listen;
  using shouter<T_rest ...>::unlisten;
  using shouter<T_rest ...>::shout;
};

template<class T_message>
class shouter<T_message> {
    std::list<typename T_message::handler> handlers;
    std::mutex mutex;

protected:      
    template<class T, class ... T_args>
    std::enable_if_t<std::is_same<T_message, T>::value> 
    shout(T_args && ... args) {        
        std::list<typename T_message::handler> handlers;
        
        {
            std::lock_guard guard{ mutex };
            handlers.swap(this->handlers);
        }
        
        for(auto &handler : handlers) {
            std::invoke(handler, std::forward<T_args>(args) ...);
        }
        
        {
            std::lock_guard guard{ mutex };
            handlers.insert(handlers.end(), this->handlers.begin(), this->handlers.end());
            this->handlers.swap(handlers);
        }
    }

public:
    template<class T>
    std::enable_if_t<std::is_same<T_message, T>::value, typename T_message::listener>
    listen(typename T_message::handler handler) {
        std::lock_guard guard{ mutex };
        return this->handlers.insert(this->handlers.end(), handler);
    }

    void unlisten(typename T_message::listener &listener) {
        std::lock_guard guard{ mutex };
        this->handlers.erase(listener);
    }
};

} /* namespace fuss */

#endif /* FUSS_HPP */
