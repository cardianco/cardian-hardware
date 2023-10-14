#pragma once

#include <vector>
#include <string>

#include "attachable.h"
#include "serial.h"

#include "../mcu/timer.h"

namespace cardian::modules {
using core::_string;

struct response {
    enum type{ NONE, OK, ERROR, STATUS };

    response(_string d, type t = type::OK):data{d},type{t}{}
    response(type t = type::OK):data{},type{t}{}

    _string data;
    type type;
};

class sim final: public core::attachable {
public:
    core::attachable& run(_string command = "at") {
        mSerial.write(command);

        static core::attachable _a;

        mSerial.onReadyRead([](const core::_any_list &args){
            auto data = args[0].cast<_string>();
            if(data.has_value()) _a.emit("then", args);
            else _a.emit("fail", args);
        });

        return _a;
    }

    void read() {
        response res{};

        while(mSerial.available()) {
            _string value = mSerial.readLine();
            _string _v = value.tolower();

            if(_v == "ok") {
                res.type = response::OK;
            } else if(_v == "error") {
                res.type = response::ERROR;
            } else if(_v != "call ready" && _v != "sms ready") {
                res.data += _v;
            }
        }

        emit("readyRead", {res});
    }

    void call(_string pn) {
        /// OK
        run(_string("ATD{0};\r\n").arg(pn));
    }

    /// $ OK
    void endCall() { run("ATH"); }

    /**
     * @brief sms
     * @abstract
     * @param phoneNumber
     * @param message
     */
    void sms(const _string &phoneNumber, const _string &message) {
        run("AT+CMGF=1")
            .then([this, &phoneNumber, &message](const core::_any_list &args) {
                _UNUSED(args)

                run("AT+CMGS=\"" + phoneNumber + "\"\n" + message + "\x1a");
            });
    }

    /**
     * @brief Reset the module.
     * @abstract Switch to airplane mode, then switch back to full functionality.
     */
    void reset() {
        /// o: OK
        run("AT+CFUN=0").then([this](const core::_any_list &args) {
            _UNUSED(args)
            mcu::timer::once(30'000, [this] {
                /// o: OK
                run("AT+CFUN=1");
            });
        });
    }

    /// $ <ccid-code>\nOK
    void getCCID() { run("AT+CCID"); }

    /// $ +CPIN: READY\nOK
    void checkPin() { run("AT+CPIN?"); }

    /// $ +CREG: <state>,<m>\nOK
    void checkRegistration() { run("AT+CREG?"); }

    /// $ +CGATT: <state>\nOK
    void checkGprsConnection() { run("AT+CGATT?"); }

    /// $ +HTTPSTATUS: GET,<x>,<y>,<z>\nOK
    void checkHttpStatus() { run("AT+HTTPSTATUS?"); }

    /// $ +CSQ: <signal>,<n>\nOK
    void checkSignal() { run("AT+CSQ"); }

    /// $ +CPAS: <state>\nOK
    void checkPhoneState() { run("AT+CPAS"); }

    /// $ +CROAMING: <state>\nOK
    void checkRoaming() { run("AT+CROAMING"); }

    void enableTcp() {
        /// $ OK
        run(R"(AT+CSTT="mtnirancell","","")")
            .then([this](const core::_any_list &args) {
                _UNUSED(args)
                /// $ OK
                run("AT+CIICR");
            });
    }

    void getIp() {
        /// $ <IP>
        run("AT+CIFSR").then([](const core::_any_list &args) {
            _UNUSED(args)
            auto val = args.size() ? args[0].cast<_string>() : "";
        });
    }

    void sendTcpData(const _string &base, const _string &url, const _string &content) {
        _string post =
            "POST {0} HTTP/1.0\n"
            "HOST: {1}\n"
            "Content-type: application/json\n"
            "Content-length: {2}\n{3}\n";

        post.arg(base).arg(url).arg(content.length()).arg(content);

        /// $ OK
        run(_string(R"(AT+CIPSTART="TCP","{0}",80)").arg(base)).then([this, &post](const core::_any_list &args) {
            _UNUSED(args)
            /// $ OK\nCONNECT OK
            run("AT+CIPSEND=" + std::to_string(post.length()));
        }).then([](const core::_any_list &args) {
            _UNUSED(args)
        });
    }

    void setFunctionality(char mode = '1', bool reset = false) {
        /// $ OK
        run(_string("AT+CFUN={0}{1}").arg(mode).arg(reset ? ",1" : ""));
    }

    /// $ OK
    void powerOff(bool urgent = false) { run(_string("AT+CPOWD={0}").arg(!urgent)); }
    /// $ OK
    void factoryReset() { run("AT&F"); }
    /// $ OK
    void writeConfig() { run("AT&W"); }

    std::vector<response> mResponses;
    core::serial1 mSerial;
};
}
