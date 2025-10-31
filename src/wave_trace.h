/*
 
 Copyright (c) 2025, Jose R. Garcia (jg-fossh@protonmail.com)
 All rights reserved.

 
    Licensed under the Apache License, Version 2.0 (the
    "License"); you may not use this file except in
    compliance with the License.  You may obtain a copy of
    the License at
 
        http://www.apache.org/licenses/LICENSE-2.0
 
    Unless required by applicable law or agreed to in
    writing, software distributed under the License is
    distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.  See
    the License for the specific language governing
    permissions and limitations under the License.

--------------------------------------------------------------------------------
File name    : wave_trace.h
Author       : Jose R Garcia (jg-fossh@protonmail.com)
Project Name : UVMSC Wace Trace
Module Name  : wave_trace
Description  : 

Additional Comments:
   
*/
#ifndef WAVE_TRACER_H_
#define WAVE_TRACER_H_

#include <systemc>
#include <uvm>

#include <verilated.h>
#include <verilated_fst_sc.h>

//
template<typename T> class wave_trace : public uvm::uvm_component {
public:

    T* uut;
    VerilatedFstSc* tfp;
    std::string uvmtest_name;

    UVM_COMPONENT_UTILS(wave_trace);


    wave_trace(uvm::uvm_component_name name= "wave", T* uut=NULL) : uvm::uvm_component(name), 
    uut(uut) {}

    virtual void build_phase(uvm::uvm_phase& phase){
      uvm::uvm_component::build_phase(phase);
      tfp = new VerilatedFstSc;

      if(!uvm::uvm_config_db<std::string>::get(this, "*", "uvmtest_name", uvmtest_name))
       UVM_FATAL("NOTST", "Test name not in the config_db: " + get_full_name() + ".uvmtest_name");

      Verilated::mkdir((uvmtest_name+"/").c_str());
    }


    virtual void start_of_simulation_phase(uvm::uvm_phase& phase) { 
        uvm::uvm_component::start_of_simulation_phase(phase);

        UVM_INFO(get_name()+"::"+__func__, "Tracing UUT into wave.fst", uvm::UVM_FULL);
        uut->trace(tfp, 99);
        tfp->open((uvmtest_name+"/wave.fst").c_str());
    }

    virtual void final_phase(uvm::uvm_phase& phase) { 
        uvm::uvm_component::final_phase(phase);

        UVM_INFO(get_name()+"::"+__func__, "Flushing FST buffer and stopping trace.", uvm::UVM_FULL);

        tfp->flush(); // Ensures that all pending data is written
        tfp->close();
        delete tfp;
    }

    virtual void pre_abort() { 
        UVM_INFO(get_name()+"::"+__func__, "Flushing FST buffer and stopping trace.", uvm::UVM_FULL);
     
        tfp->flush(); // Ensures that all pending data is written
        tfp->close();
        delete tfp;
    }
};

#endif /* RESET_GENERATOR_H_ */

