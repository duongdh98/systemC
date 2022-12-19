#ifndef __CUSTOM_GEN_H
#define __CUSTOM_GEN_H

#include <systemc.h>

typedef struct {
    sc_uint <8> n_value;
    sc_uint <8> m_value;

} values_t;

class CUSTOM_GEN : public sc_module {

public:
    SC_HAS_PROCESS(CUSTOM_GEN);
    CUSTOM_GEN(sc_module_name name, int nseconds);
    ~CUSTOM_GEN(void);

    int ns;
    sc_in_clk                clk;

private:

};

#endif /* __CUSTOM_GEN_H */