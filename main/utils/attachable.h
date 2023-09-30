#pragma once

#include <functional>
#include <algorithm>
#include <optional>
#include <vector>
#include <map>
#include <any>

#include "core.h"

namespace cardian::core {
struct event;
class attachable;

using _any_list = std::vector<_any>;
using _callback = std::function<void(const _any_list&)>;

struct event {
    enum Type {Default, Once, Exclusive};
    Type type = Type::Default;
    _callback func;
};

class attachable {
public:
    void emit(const _string &name, const _any_list &args = {}) {
        if(mEvents.count(name)) {
            std::vector<event> &elist = mEvents[name];

            for(event &e: elist) {
                std::invoke(e.func, args);
                if(e.type == event::Exclusive) break;
            }

            if(elist.size() && elist.front().type == event::Exclusive) {
                elist.erase(elist.begin());
            } else {
                auto last = std::remove_if(elist.begin(), elist.end(), [](event &e){
                    return e.type == event::Once;
                });
                elist.resize(std::distance(elist.begin(), last));
            }
        }
    }

    void on(const _string &name, const _callback &func, event::Type type = event::Default) {
        std::vector<event> &elist = mEvents[name];
        if(type == event::Exclusive) {
            auto iter = std::find_if(elist.begin(), elist.end(), [](event &e){ return e.type != event::Exclusive; });
            elist.insert(iter, { type, func });
        } else {
            elist.push_back({ type, func });
        }
    }

    void clear(const _string &name) {
        mEvents[name].clear();
    }

    attachable& then(const _callback &func) {
        on("then", func, event::Exclusive);
        return *this;
    }

    attachable& fail(const _callback &func) {
        on("fail", func, event::Exclusive);
        return *this;
    }

    size_t count() const { return mEvents.size(); }

    static void _NONE(const _any_list&) {}

private:
    std::map<_string, std::vector<event>> mEvents;
};

inline static attachable _attachable;
}
