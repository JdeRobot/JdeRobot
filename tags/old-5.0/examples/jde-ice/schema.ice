module jde{
  sequence<byte> img;

  interface Schema {
    int start();
    int stop();
    int iteration();
    img getData();
  };
};
