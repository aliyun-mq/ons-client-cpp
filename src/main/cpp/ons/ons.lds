VERSION_3.0.0 {
    global:
        *rocketmq::*;
        *ons::*;
        extern "C++" {
            *rocketmq::*;
            *ons::*;
        };
    local: 
        *;
};