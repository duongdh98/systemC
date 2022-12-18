#include "include/custom_gen.hpp"
#include "include/pwm.hpp"

// sc_main in top level function like in C++ main
int sc_main(int argc, char* argv[]) {

  cout << "\n\n=== Begin Program of systemC  ========================================================== \n\n";
#if 0
  /* #1> Example counter */
  sc_signal<bool>   clock;
  sc_signal<bool>   reset;
  sc_signal<bool>   enable;
  sc_signal< sc_uint<8> > counter_out;

  // Connect the DUT
  counter zcounter("COUNTER");
  zcounter.clock(clock);
  zcounter.reset(reset);
  zcounter.enable(enable);
  zcounter.counter_out(counter_out);

  sc_start(1, SC_NS);

  // Open VCD file
  sc_trace_file *wf = sc_create_vcd_trace_file("zWave");
  // Dump the desired signals
  sc_trace(wf, clock, "clock");
  sc_trace(wf, reset, "reset");
  sc_trace(wf, enable, "enable");
  sc_trace(wf, counter_out, "count");

  // Initialize all variables
  reset = 0;       // initial value of reset
  enable = 1;  // Assert enable
  for (int i = 0; i < 20; i++) {
    clock = 0; 
    sc_start(1, SC_NS);
    clock = 1; 
    sc_start(1, SC_NS);
  }


  cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
  sc_close_vcd_trace_file(wf);

#endif

    /* Example pwm */
#if 1

    sc_signal<bool>         signal_clk;    
    sc_signal< sc_uint<8> > m_value;
    sc_signal< sc_uint<8> > n_value;
    sc_signal<bool>         reset;
    sc_signal<bool>         wr_enable;
    sc_signal<bool>         pwm_out;
    sc_signal<bool>         pwm_time;

    CUSTOM_GEN custom_gen("custom_gen", 100);
    custom_gen.clk(signal_clk);
    custom_gen.wr_value_m(m_value);
    custom_gen.wr_value_n(n_value);

    PWM pwm("pwm");
    pwm.pwm_clk(signal_clk);
    pwm.pwm_reset(reset);
    pwm.pwm_wr_enable(wr_enable);
    pwm.pwm_wr_m_value(m_value);
    pwm.pwm_wr_n_value(n_value);
    pwm.pwm_out(pwm_out);
    pwm.pwm_time(pwm_time);

    /*  */
    wr_enable = true;

    sc_start(1, SC_NS);
    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("zWave");

    // Dump the desired signals
    sc_trace(wf, signal_clk, "signal_clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, wr_enable, "wr_enable");
    sc_trace(wf, m_value, "m_value");
    sc_trace(wf, n_value, "n_value");
    sc_trace(wf, pwm_out, "pwm_out");
    sc_trace(wf, pwm_time, "pwm_time");

    for (int i = 0; i < custom_gen.ns; i++)
    {
        signal_clk = 0; 
        sc_start(1, SC_NS);
        signal_clk = 1; 
        sc_start(1, SC_NS);
    }
    
    sc_close_vcd_trace_file(wf);

#endif

  return 0;// Terminate simulation
  cout << "\n\n=== End Program of systemC    ==========================================================";
  return(0);
}