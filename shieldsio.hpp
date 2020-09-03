#ifndef SHIELDSIO_HPP
#define SHIELDSIO_HPP

class ShieldsIO {
  public:
  String endpoint;
  String label, message, color;
  ShieldsIO(String endpoint);
  void fetch();
};

#endif
