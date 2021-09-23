#ifndef INPUTS_HPP
#define INPUTS_HPP

class Inputs {
   public:
    [[nodiscard]] static Inputs *get_instance() {
        static Inputs inputs;
        return &inputs;
    }

    static void press(const int n) {
        get_instance()->m_states[n] = true;
    }

    static void release(const int n) {
        get_instance()->m_states[n] = false;
    }

    [[nodiscard]] static bool is_pressed(const int n) {
        return get_instance()->m_states[n];
    }

   private:
    Inputs() : m_states{} {
    }

    bool m_states[512];
};

#endif
