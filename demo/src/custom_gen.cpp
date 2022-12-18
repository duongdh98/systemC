#include "../include/custom_gen.hpp"

CUSTOM_GEN::CUSTOM_GEN(sc_module_name name, int nseconds) : sc_module(name), ns(nseconds)
{
    init();
}

CUSTOM_GEN::~CUSTOM_GEN() {

}

void CUSTOM_GEN::init() {

    cout << __FUNCTION__ << ": init ... \n";

    wr_value_m.initialize(false);
    wr_value_n.initialize(false);

    SC_METHOD(write_values_method);
    sensitive << clk.pos();
    dont_initialize();
}

values_t CUSTOM_GEN::gen_values() {
    
    values_t val;
    val.m_value = rand() % 10 + 1;
    val.n_value = rand() % val.m_value + 1;

    return val;
}

void CUSTOM_GEN::write_values_method() {

    values_t val;
    val = gen_values();
    wr_value_m.write(val.m_value);
    wr_value_n.write(val.n_value);
}