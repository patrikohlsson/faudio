
#pragma once

#include <algorithm>
#include <utility>
#include <scl/utility.hpp>
#include <scl/audio/audio_types.hpp>
#include <scl/audio/processor.hpp>

namespace scl
{
  namespace audio
  {
    namespace processor
    {
      using std::pair;

      // template <class A, class B>
      // class constant_processor
      //   : public processor < void, void, void,
      //     void, void,
      //     A, B >
      // {
      // public:
      //   constant_processor(output_type value)
      //     : value(value) {}
      // 
      //   void prepare(const argument_type& argument)
      //   {
      //   }
      //   void cleanup(result_type& result)
      //   {
      //   }
      //   void load(const state_type& state)
      //   {
      //   }
      //   void store(state_type& state)
      //   {
      //   }
      //   bool is_ready()
      //   {
      //     return value ? true : false;
      //   }
      //   void process(const list<input_message_type>& input_messages,
      //                const input_type& input,
      //                output_type& output,
      //                list<output_message_type>& output_messages)
      //   {
      //     output = value;
      //   }
      // 
      // private:
      //   output_type value;
      // };  







      class raw_constant_processor : public raw_processor
      {         
        std::unique_ptr<char> value;
      public:
        raw_constant_processor(intptr_t input, size_t size)
          : raw_processor(0, 0, 0, 0, size)
        {                          
          value.reset(new char[size]);
          scl::raw_copy(input, input + size, (intptr_t) value.get());
        }
        void prepare(intptr_t argument) {}
        void cleanup(intptr_t argument) {}
        void load(intptr_t argument) {}
        void store(intptr_t argument) {}
        bool is_ready()
        {
          return true;
        }
        void process(intptr_t input_messages,
                     intptr_t input,
                     intptr_t output,
                     intptr_t output_messages)
        {
          size_t size = raw_processor::output_size;
          intptr_t val = (intptr_t) (value.get());
          scl::raw_copy(val, val + size, output);
        }
      };

    }
  }
}

