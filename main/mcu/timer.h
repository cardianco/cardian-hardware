#pragma once

#include <map>
#include <vector>
#include <functional>
#include <cstdint>

#include <system_stm32f1xx.h>

#include "mcu.h"

#define _UNUSED(x) (void)(x);
#undef emit

namespace cardian::mcu {

template <typename _ret = void, typename ... _arg>
using _callback = std::function<_ret(_arg ...)>;

struct tevent {
	enum Type {Default, Once, Exclusive};
	uint32_t interval;
	_callback<> callback;
	Type type = Type::Default;
};

class timer {
public:
	void attach(uint32_t interval, const _callback<> &callback = []{}, tevent::Type type = tevent::Default) {
		attach({interval, callback, type});
	}

	void attach(const tevent &timerEvent) {
		mEvents[now() + timerEvent.interval].push_back(timerEvent);
		timerloopEnable();
	}

	void timerloop() {
		uint64_t nextEvent = mEvents.begin()->first;

		if(!mEvents.empty() && nextEvent < now()) {
			for(auto &_event: mEvents.begin()->second) {
				if(_event.type == tevent::Default) {
					// Repeat timer event
					attach(_event);
				}
				_event.callback();
			}

			mEvents.erase(mEvents.begin()->first);

			if(mEvents.empty()) {
				timerloopStop();
			} else {
				mCounter = now();
				TIMx->EGR |= TIM_EGR_UG; /// Reset TIMx
				TIMx->CNT = 0;
			}
		}
	}

	uint64_t now() { return TIMx->CNT + mCounter; }

    /**
     * @brief once
     * @abstract Calls a function at specified intervals once.
     * @param interval
     * @param callback
     */
    static void once(uint32_t interval, const _callback<> &callback) {
        instance().attach(interval, callback, tevent::Once);
    }

    /**
     * @brief repeat
     * @abstract Repeat calling a function at specified intervals.
     * @param interval in milisecond
     * @param callback
     */
    static void repeat(uint32_t interval, const _callback<> &callback) {
        instance().attach(interval, callback, tevent::Default);
    }

	/**
	 * @brief instance
	 * @return singleton instance of timer.
	 */
	static timer& instance() {
		static timer _timer;
		return _timer;
	}

	inline static TIM_TypeDef * TIMx = TIM3;

private:
	void timerloopEnable() {
		if(!(TIMx->CR1 & TIM_CR1_CEN)) {
			uint32_t num = utils::timxNum(TIMx);
			utils::timxInit(num - 1);
			TIMx->ARR = UINT32_MAX;
			TIMx->CR1 |= TIM_CR1_CEN;
		}
	}

	void timerloopStop() {
		TIMx->CR1 &= ~TIM_CR1_CEN;
		TIMx->EGR |= TIM_EGR_UG; /// Reset TIMx
		TIMx->CNT = 0;
		mCounter = 0;
	}

	std::map<uint64_t, std::vector<tevent>> mEvents;
	uint64_t mCounter = 0;
};
}

