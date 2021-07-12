 
#ifndef __OS__DRIVERS__DRIVER_H
#define __OS__DRIVERS__DRIVER_H

namespace os
{
    namespace drivers
    {
        
        class Driver 
        {
        public:
            Driver();
            ~Driver();
            
            virtual void Activate();
            virtual int Reset();
            virtual void Deactivate();
        };
        
        class DriverManager
        {
        private:
            Driver* drivers[256];
            int numDrivers;
            
        public:
            DriverManager();
            void AddDriver(Driver*);
            
            void ActivateAll();
        };
    }
}

#endif
