#ifndef __SINGLETION_H__
#define __SINGLETION_H__

#include <memory>
#include <mutex>

#undef PATTERN_SINGLETION_DECLARE
#define PATTERN_SINGLETION_DECLARE(classname)           \
public:                                                 \
    static std::shared_ptr<classname> instance();       \
    virtual ~classname();                               \
private:                                                \
    classname();                                        \
    classname(const classname&) = delete;               \
    classname& operator=(const classname&) = delete;    \

#undef PATTERN_SINGLETION_IMPLEMENT
#define PATTERN_SINGLETION_IMPLEMENT(classname)         \
std::shared_ptr<classname> classname::instance()        \
{                                                       \
    static std::shared_ptr<classname> instance##classname(nullptr);             \
    if (!instance##classname)                                                   \
    {                                                                           \
        static std::mutex s_mutex##classname;                                   \
        std::lock_guard<std::mutex> lock(s_mutex##classname);                   \
        if (!instance##classname)                                               \
        {                                                                       \
            instance##classname = std::shared_ptr<classname>(new classname);    \
        }                                                                       \
        return instance##classname;                                             \
    }                                                                           \
}                                                                               \

#endif