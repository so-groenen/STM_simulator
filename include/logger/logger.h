#ifndef STM_LOGGER_H
#define STM_LIGGER_H

#if defined(LOGLEVEL_DEBUG) 
    #define LOG_DEBUG(X) ( printf("STMSimulator: DEBUG: %s", X) )
#else
    #define LOG_DEBUG(X)
#endif

#if defined(LOGLEVEL_INFO) || defined(LOGLEVEL_DEBUG)  

    #define LOG_INFO(X)  ( printf("STMSimulator: INFO: %s", X) )
    #else
    #define LOG_INFO(X)
#endif


#if defined(LOGLEVEL_WARNING) || defined(LOGLEVEL_INFO) || defined(LOGLEVEL_DEBUG)  
    #define LOG_WARNING(X)  ( printf("STMSimulator: WARNING: %s", X) )
#else
    #define LOG_WARNING(X)
#endif






#endif