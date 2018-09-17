

#include "NodeApp.h"
#include "NodeXmrigCpu.h"

#include <iostream>



Nan::Persistent<v8::FunctionTemplate> NodeXmrigCpu::constructor;

NAN_MODULE_INIT(NodeXmrigCpu::Init) {
        v8::Local<v8::FunctionTemplate> ctor = Nan::New<v8::FunctionTemplate>(NodeXmrigCpu::New);
        constructor.Reset(ctor);
        ctor->InstanceTemplate()->SetInternalFieldCount(1);
        ctor->SetClassName(Nan::New("NodeXmrigCpu").ToLocalChecked());

        // link our getters and setter to the object property
        Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("jsonConfig").ToLocalChecked(), NodeXmrigCpu::HandleGetters, NodeXmrigCpu::HandleSetters);
        Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("rewardAddress").ToLocalChecked(), NodeXmrigCpu::HandleGetters, NodeXmrigCpu::HandleSetters);
        Nan::SetAccessor(ctor->InstanceTemplate(), Nan::New("numberOfCores").ToLocalChecked(), NodeXmrigCpu::HandleGetters, NodeXmrigCpu::HandleSetters);

        Nan::SetPrototypeMethod(ctor, "startMining", startMining);
        Nan::SetPrototypeMethod(ctor, "stopMining", stopMining);
        Nan::SetPrototypeMethod(ctor, "getStatus", getStatus);


        target->Set(Nan::New("NodeXmrigCpu").ToLocalChecked(), ctor->GetFunction());
}

NAN_METHOD(NodeXmrigCpu::New) {
        // throw an error if constructor is called without new keyword
        if(!info.IsConstructCall()) {
          return Nan::ThrowError(Nan::New("NodeXmrigCpu::New - called without new keyword").ToLocalChecked());
        }

        // expect exactly 2 arguments
        if(info.Length() != 2) {
          return Nan::ThrowError(Nan::New("NodeXmrigCpu::New - expected arguments jsonConfig, number of cores").ToLocalChecked());
        }

        // expect arguments to be numbers
        if(!info[0]->IsString() || !info[1]->IsNumber()) {
          return Nan::ThrowError(Nan::New("NodeXmrigCpu::New - expected arguments to be string, number").ToLocalChecked());
        }

        Nan::Utf8String configUtf8Value(info[0]);
        int len = configUtf8Value.length();
        std::string jsonConfig = std::string(*configUtf8Value, len);

        int numberOfCores = info[1]->NumberValue();

        // create a new instance and wrap our javascript instance
        NodeXmrigCpu* miner = new NodeXmrigCpu(jsonConfig, numberOfCores);
        miner->Wrap(info.Holder());


        std::cout << "New xmrcpuring: jsonConfig" << miner->jsonConfig << " number of Cores:" << miner->numberOfCores << std::endl;


        // return the wrapped javascript instance
        info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(NodeXmrigCpu::startMining) {
        // unwrap this NodeXmrigCpu
        NodeXmrigCpu *self = Nan::ObjectWrap::Unwrap<NodeXmrigCpu>(info.This());

        std::cout << "Starting mining..." << std::endl;
        self->minerApp->exec();

        std::cout << "Mining started" << std::endl;

        info.GetReturnValue().Set(Nan::New("true").ToLocalChecked());
}

NAN_METHOD(NodeXmrigCpu::stopMining) {
        NodeXmrigCpu *self = Nan::ObjectWrap::Unwrap<NodeXmrigCpu>(info.This());

        std::cout << "Closing miner!" << std::endl;
        self->minerApp->close();
        std::cout << "Miner closed" << std::endl;

        info.GetReturnValue().Set(Nan::New("true").ToLocalChecked());
}

NAN_METHOD(NodeXmrigCpu::getStatus) {
        // unwrap this NodeXmrigCpu
        NodeXmrigCpu *self = Nan::ObjectWrap::Unwrap<NodeXmrigCpu>(info.This());

        std::string result = self->minerApp->getStatus();

        info.GetReturnValue().Set(Nan::New(result).ToLocalChecked());
}

NAN_GETTER(NodeXmrigCpu::HandleGetters) {
        NodeXmrigCpu* self = Nan::ObjectWrap::Unwrap<NodeXmrigCpu>(info.This());

        std::string propertyName = std::string(*Nan::Utf8String(property));
        if (propertyName == "jsonConfig") {
          v8::Local<v8::Value> returnValue = Nan::CopyBuffer((char*)self->jsonConfig.c_str(), self->jsonConfig.size()).ToLocalChecked();
          info.GetReturnValue().Set(returnValue);
        } else if (propertyName == "numberOfCores") {
          info.GetReturnValue().Set(self->numberOfCores);
        } else {
          info.GetReturnValue().Set(Nan::Undefined());
        }
}

NAN_SETTER(NodeXmrigCpu::HandleSetters) {
        NodeXmrigCpu* self = Nan::ObjectWrap::Unwrap<NodeXmrigCpu>(info.This());

        if(!value->IsString()) {
          return Nan::ThrowError(Nan::New("expected value to be a string").ToLocalChecked());
        }

//        std::string propertyName = std::string(*Nan::Utf8String(property));
//        if (propertyName == "pool") {
//          Nan::Utf8String poolUtf8Value(value);
//          int len = poolUtf8Value.length();
//          self->pool = std::string(*poolUtf8Value, len);
//        } else if (propertyName == "rewardAddress") {
//          Nan::Utf8String rewardAddressUtf8Value(value);
//          int len = rewardAddressUtf8Value.length();
//          self->rewardAddress = std::string(*rewardAddressUtf8Value, len);
//        } else if (propertyName == "numberOfCores") {
//          self->numberOfCores = value->NumberValue();
//        }


}