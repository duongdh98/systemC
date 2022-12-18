#ifndef __CUSTOM_GEN_H
#define __CUSTOM_GEN_H

#include "../include/global.hpp"

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
    sc_out < sc_uint<8> >    wr_value_n;
    sc_out < sc_uint<8> >    wr_value_m;

    void init();
    values_t gen_values();
    void write_values_method();

private:

};

#endif /* __CUSTOM_GEN_H */