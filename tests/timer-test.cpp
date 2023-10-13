#include <iostream>
#include <functional>
#include <atomic>
#include <thread>

#include "../main/utils/timer.h"

using cardian::core::timer;
using hrclock = std::chrono::high_resolution_clock;
using dbl_dur = std::chrono::duration<double>;

class singleshot {
public:
    template <class callable, class... arguments>
    singleshot(int ms, callable&& f, arguments&&... args) {
        std::thread([ms, &f, &args...]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(ms));
            f(std::forward<arguments>(args)...);
        }).join();
    }
};

void TIM3_IRQn(void) { timer::instance().callNext(); }

#include "gtest/gtest.h"

TEST(timer_tests, singleshot_timer_test) {
	static auto startPoint = hrclock::now();
	static uint32_t interval = 100;

	timer::setNow([] {
		std::chrono::duration<double> n = hrclock::now() - startPoint;
		return static_cast<uint32_t>(n.count());
	});

	timer::setRescheduleCB([](uint32_t ms) {
		std::chrono::duration<double> n = hrclock::now() - startPoint;
		auto diff = n.count() * 1000;
		EXPECT_LE(diff, 10);
		/// Here are steps to set timer to fire intrrupt after <ms> milisecond.
		singleshot(ms, TIM3_IRQn);
	});

	timer::once(interval, [](cardian::core::_any_list args) {
		auto diff = dbl_dur(hrclock::now() - startPoint).count() * 1000;
		/// Expect timer to run after <interval> -+ 15  ms.
		EXPECT_LE(std::abs(diff - interval), 20);
	});
}

TEST(timer_tests, repeat_timer_test) {
	static auto startPoint = hrclock::now();
	static auto start = hrclock::now();
	static uint32_t interval = 100;
	timer &_timer = timer::instance();

	_timer._tstop = []{};
	_timer._treset = []{};
	_timer._tenable = []{};
	_timer._tcnt = []{
		std::chrono::duration<double> n = hrclock::now() - startPoint;
		return static_cast<uint32_t>(n.count() * 1000);
	};
	_timer._tarr = [](uint32_t){};

	timer::repeat(interval, [](cardian::core::_any_list args) {
		auto diff = dbl_dur(hrclock::now() - start).count() * 1000;
		EXPECT_LE(std::abs(diff - interval), 20);
	});
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
