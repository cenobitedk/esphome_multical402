#ifndef _M402_
#define _M402_

#include "esphome.h"
#include "esphome/core/component.h"

#include "kmp.h"

class Multical402 : public PollingComponent
{
public:
  Sensor *sensor_energy{nullptr};
  Sensor *sensor_power{nullptr};
  Sensor *sensor_tempin{nullptr};
  Sensor *sensor_tempout{nullptr};
  Sensor *sensor_tempdiff{nullptr};
  Sensor *sensor_flow{nullptr};
  Sensor *sensor_volume{nullptr};

  // constructor
  Multical402(
      uint32_t update_interval,
      UARTComponent *uart_bus,
      Sensor *m__energy,
      Sensor *m__power,
      Sensor *m__tin,
      Sensor *m__tout,
      Sensor *m__tdiff,
      Sensor *m__flow,
      Sensor *m__volume) : PollingComponent(update_interval),
                           sensor_energy(m__energy),
                           sensor_power(m__power),
                           sensor_tempin(m__tin),
                           sensor_tempout(m__tout),
                           sensor_tempdiff(m__tdiff),
                           sensor_flow(m__flow),
                           sensor_volume(m__volume)
  {
    _kmp = new KMP(uart_bus);
  }

  float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

  void setup() override {}

  void update() override
  {
    ESP_LOGD(TAG, "Start update");

    float energy = _kmp->HeatEnergy();
    if (energy)
      sensor_energy->publish_state(energy);

    float volume = _kmp->Volume();
    if (volume)
      sensor_volume->publish_state(volume);

    float tempin = _kmp->CurrentForwardTemperature();
    if (tempin)
      sensor_tempin->publish_state(tempin);

    float tempout = _kmp->CurrentReturnTemperature();
    if (tempout)
      sensor_tempout->publish_state(tempout);

    float tempdiff = _kmp->CurrentDifferentialTemperature();
    if (tempdiff)
      sensor_tempdiff->publish_state(tempdiff);

    float power = _kmp->CurrentPower();
    if (power)
      sensor_power->publish_state(power);

    float flow = _kmp->CurrentWaterFlow();
    if (flow)
      sensor_flow->publish_state(flow);

    ESP_LOGD(TAG, "End update");
  }

private:
  KMP *_kmp;
};

#endif