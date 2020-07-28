//
// Created by dario on 24/07/20.
//

#ifndef FFB_TESTER_OSTREAM_SDL_HPP
#define FFB_TESTER_OSTREAM_SDL_HPP
//
// Created by dario on 22/07/20.
//

static inline std::ostream &operator<<(std::ostream &printer, const SDL_HapticDirection &hd)
{
    printer << "{ \"type\": " << (uint16_t) hd.type << ", \"dir\": [" << hd.dir[0] << ',' << hd.dir[1] << ','
            << hd.dir[2] << "] }";
    return printer;
}

static inline std::ostream &operator<<(std::ostream &printer, const SDL_HapticConstant &hc)
{
    printer << "{ \"type\": " << hc.type << ", \"direction\": " << hc.direction
            << ", \"length\": " << hc.length << ", \"delay\": " << hc.delay
            << ", \"button\": " << hc.button << ", \"interval\": " << hc.interval
            << ", \"level\": " << hc.level << ", \"attack_length\": " << hc.attack_length
            << ", \"attack_level\": " << hc.attack_level << ", \"fade_length\": " << hc.fade_length
            << ", \"fade_level\": " << hc.fade_level << "}";
    return printer;
}

static inline std::ostream &operator<<(std::ostream &os, const SDL_HapticPeriodic &periodic)
{
    os << "{ \"type\": " << periodic.type << " ,\"direction\": " << periodic.direction << " ,\"length\": "
       << periodic.length
       << " ,\"delay\": " << periodic.delay << " ,\"button\": " << periodic.button << " ,\"interval\": "
       << periodic.interval
       << " ,\"period\": " << periodic.period << " ,\"magnitude\": " << periodic.magnitude << " ,\"offset\": "
       << periodic.offset
       << " ,\"phase\": " << periodic.phase << " ,\"attack_length\": " << periodic.attack_length
       << " ,\"attack_level\": "
       << periodic.attack_level << " ,\"fade_length\": " << periodic.fade_length << " ,\"fade_level\": "
       << periodic.fade_level << "}";
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const SDL_HapticCondition &condition)
{
    os << "{ \"type\": " << condition.type << ", \"direction\": " << condition.direction << ", \"length\": "
       << condition.length
       << ", \"delay\": " << condition.delay << ", \"button\": " << condition.button << ", \"interval\": "
       << condition.interval
       << ", \"right_sat\": " << condition.right_sat << ", \"left_sat\": " << condition.left_sat
       << ", \"right_coeff\": "
       << condition.right_coeff << ", \"left_coeff\": " << condition.left_coeff << ", \"deadband\": "
       << condition.deadband
       << ", \"center\": " << condition.center << '}';
    return os;
}

static inline std::ostream &operator<<(std::ostream &printer, const SDL_HapticEffect &eff)
{
    switch (eff.type)
    {
        case SDL_HAPTIC_CONSTANT:
            printer << eff.constant;
            break;
        case SDL_HAPTIC_SINE:
        case SDL_HAPTIC_TRIANGLE:
        case SDL_HAPTIC_SAWTOOTHUP:
        case SDL_HAPTIC_SAWTOOTHDOWN:
            printer << eff.periodic;
            break;
        case SDL_HAPTIC_SPRING:
        case SDL_HAPTIC_FRICTION:
        case SDL_HAPTIC_DAMPER:
        case SDL_HAPTIC_INERTIA:
            printer << eff.condition;
            break;
        default:
            printer << "Error";
            break;
    }
    return printer;
}

#endif //FFB_TESTER_OSTREAM_SDL_HPP
