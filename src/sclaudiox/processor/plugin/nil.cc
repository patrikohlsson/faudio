/*
    ScoreCleaner Audio Engine
    
    Copyright (c) 2012 DoReMIR Music Research AB.
    All rights reserved.
 */
/**
    @file   sclaudiox/processor/plugin/nil.cc
    @author Hans Hoglund
 */

// specific includes

#include "sclaudiox/processor/plugin/nil.h"
#include "sclaudiox/util/misc.h"


namespace doremir {
namespace scl {

struct NilPluginDescriptionData
{                          
    NilPlugin* plugin;
    // info
};
struct NilPluginProcessorData
{
    NilPlugin* plugin;
    NilPluginDescription* description;
    // instance
};
struct NilPluginData
{
    NilPluginDescription* description;
    // plugin
};


NilPluginDescription::NilPluginDescription(NilPluginDescriptionData* data)
    : mData(data) {}
    
NilPluginDescription::~NilPluginDescription()
{
    delete mData;
}

String NilPluginDescription::name() const
{         
    // TODO implement
}                  
bool NilPluginDescription::isAtomic() const
{
    // TODO implement
}
bool NilPluginDescription::isStateful() const
{
    // TODO implement
}
bool NilPluginDescription::isPlugin() const
{
    // TODO implement
}

int NilPluginDescription::numberOfInputs() const
{
    // TODO implement
}

int NilPluginDescription::numberOfOutputs() const
{
    // TODO implement
}

int NilPluginDescription::numberOfBuses() const
{
    // TODO implement
}


AudioPlugin* NilPluginDescription::plugin()
{
    // TODO implement
}              




NilPluginProcessor::NilPluginProcessor(NilPluginProcessorData* data)
    : mData(data) {}
    
NilPluginProcessor::~NilPluginProcessor()
{
    delete mData;
}


AudioProcessorDescription* NilPluginProcessor::description() const
{
    // TODO implement
}

void NilPluginProcessor::accept(Message message)
{
    // TODO implement
}

void NilPluginProcessor::prepare(AudioProcessingInformation& info, AudioProcessingBuffer &signal)
{
    // TODO implement
}

void NilPluginProcessor::process(AudioProcessingInformation& info, AudioProcessingBuffer &signal)
{
    // TODO implement
}

void NilPluginProcessor::cleanup(AudioProcessingInformation& info, AudioProcessingBuffer &signal)
{
    // TODO implement
}

AudioPlugin* NilPluginProcessor::plugin()
{
    // TODO implement
}
    
void* NilPluginProcessor::nativePluginInstance()
{
    // TODO implement
}
                                      



NilPlugin::NilPlugin(NilPluginData* data)
    : mData(data) {}

NilPlugin::~NilPlugin()
{
    delete mData;
}

PluginAudioProcessorDescription* NilPlugin::description()
{
    return vst2PluginDescription();
}

PluginAudioProcessor* NilPlugin::createProcessor()
{
    return createNilPluginProcessor();
}               

void* NilPlugin::nativePlugin()
{
    // TODO implement
}

NilPluginDescription* NilPlugin::vst2PluginDescription()
{                       
    if(!mData->description)
    {                                                   
        // create and init description
    }   
    return mData->description;
}

NilPluginProcessor* NilPlugin::createNilPluginProcessor()
{
    // TODO implement
} 

std::list<NilPlugin*> NilPlugin::vst2Plugins(FilePath path)
{                                                
    // TODO implement
}

} // namespace
} // namespace                    
