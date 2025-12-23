// Auto-generated Random Forest Decision Tree for ESP32
// Generated from trained sklearn RandomForestClassifier
// Accuracy: 80.61%
// Model: data/models/random_forest.pkl

#ifndef RANDOM_FOREST_MODEL_H
#define RANDOM_FOREST_MODEL_H

// Feature indices (must match training order)
// 0-13: Time domain features
// 14-23: Frequency domain features  
// 24-29: Wavelet features

const char* FEATURE_NAMES[30] = {
  "mean", "std", "var", "min", "max", "range", "median",
  "skewness", "kurtosis", "q25", "q75", "mean_abs_diff",
  "std_diff", "max_diff", "fft_low", "fft_mid", "fft_high",
  "fft_total", "fft_spread", "fft_range", "fft_dom", "fft_peak",
  "fft_centroid", "fft_rolloff", "wav_approx_std", "wav_detail1_std",
  "wav_detail2_std", "wav_detail3_std", "wav_detail1_energy",
  "wav_detail2_energy"
};

// Activity class names
const char* activity_names[6] = {
  "IDLE", "SOCIAL", "VIDEO", "MESSAGE", "BROWSE", "GAMING"
};

// Decision tree classifier (extracted from Random Forest)
int classify_random_forest(float* features) {
if (features[27] <= 24.434164f) {
  if (features[18] <= 141557.484375f) {
    return 2;  // VIDEO
  } else {
    if (features[23] <= 0.842603f) {
      if (features[6] <= 1430.200012f) {
        if (features[15] <= 141829.867188f) {
          if (features[0] <= 1408.258057f) {
            if (features[19] <= 6094.030518f) {
              if (features[18] <= 144859.960938f) {
                return 2;  // VIDEO
              } else {
                return 0;  // IDLE
              }
            } else {
              if (features[21] <= 1.344099f) {
                if (features[29] <= 519.533112f) {
                  if (features[27] <= 13.154297f) {
                    return 0;  // IDLE
                  } else {
                    if (features[10] <= 1430.906250f) {
                      if (features[15] <= 140314.921875f) {
                        return 1;  // SOCIAL
                      } else {
                        return 0;  // IDLE
                      }
                    } else {
                      return 1;  // SOCIAL
                    }
                  }
                } else {
                  if (features[6] <= 1428.387512f) {
                    if (features[23] <= 0.834019f) {
                      if (features[28] <= 340.791794f) {
                        return 0;  // IDLE
                      } else {
                        return 0;  // IDLE
                      }
                    } else {
                      return 1;  // SOCIAL
                    }
                  } else {
                    return 1;  // SOCIAL
                  }
                }
              } else {
                if (features[9] <= 1358.179993f) {
                  if (features[10] <= 1431.609985f) {
                    return 1;  // SOCIAL
                  } else {
                    return 3;  // MESSAGE
                  }
                } else {
                  if (features[0] <= 1406.747131f) {
                    if (features[26] <= 23.035474f) {
                      if (features[11] <= 15.241717f) {
                        return 0;  // IDLE
                      } else {
                        return 3;  // MESSAGE
                      }
                    } else {
                      return 1;  // SOCIAL
                    }
                  } else {
                    return 1;  // SOCIAL
                  }
                }
              }
            }
          } else {
            if (features[21] <= 1.280524f) {
              if (features[26] <= 21.206798f) {
                if (features[25] <= 27.639824f) {
                  if (features[29] <= 315.393967f) {
                    return 1;  // SOCIAL
                  } else {
                    if (features[17] <= 19810.951172f) {
                      return 0;  // IDLE
                    } else {
                      return 0;  // IDLE
                    }
                  }
                } else {
                  if (features[29] <= 390.263702f) {
                    return 3;  // MESSAGE
                  } else {
                    return 1;  // SOCIAL
                  }
                }
              } else {
                if (features[18] <= 145894.625000f) {
                  return 3;  // MESSAGE
                } else {
                  return 4;  // BROWSE
                }
              }
            } else {
              if (features[28] <= 572.704407f) {
                if (features[6] <= 1427.784973f) {
                  if (features[13] <= 94.684998f) {
                    if (features[0] <= 1408.910645f) {
                      return 1;  // SOCIAL
                    } else {
                      if (features[12] <= 27.996636f) {
                        return 0;  // IDLE
                      } else {
                        return 0;  // IDLE
                      }
                    }
                  } else {
                    if (features[18] <= 145788.140625f) {
                      return 0;  // IDLE
                    } else {
                      if (features[23] <= 0.799597f) {
                        return 0;  // IDLE
                      } else {
                        if (features[25] <= 32.044001f) {
                          return 3;  // MESSAGE
                        } else {
                          return 3;  // MESSAGE
                        }
                      }
                    }
                  }
                } else {
                  if (features[1] <= 35.534641f) {
                    if (features[26] <= 31.693027f) {
                      if (features[15] <= 140851.992188f) {
                        return 0;  // IDLE
                      } else {
                        if (features[23] <= 0.808017f) {
                          if (features[11] <= 17.265252f) {
                            if (features[28] <= 505.807785f) {
                              return 3;  // MESSAGE
                            } else {
                              return 4;  // BROWSE
                            }
                          } else {
                            return 4;  // BROWSE
                          }
                        } else {
                          if (features[16] <= 3072.484863f) {
                            if (features[13] <= 87.434998f) {
                              return 3;  // MESSAGE
                            } else {
                              return 4;  // BROWSE
                            }
                          } else {
                            if (features[8] <= 0.306338f) {
                              return 3;  // MESSAGE
                            } else {
                              return 3;  // MESSAGE
                            }
                          }
                        }
                      }
                    } else {
                      return 4;  // BROWSE
                    }
                  } else {
                    return 1;  // SOCIAL
                  }
                }
              } else {
                return 1;  // SOCIAL
              }
            }
          }
        } else {
          if (features[6] <= 1428.994995f) {
            if (features[10] <= 1431.710022f) {
              if (features[15] <= 142224.367188f) {
                if (features[25] <= 33.699631f) {
                  if (features[23] <= 0.665362f) {
                    return 0;  // IDLE
                  } else {
                    if (features[25] <= 17.376893f) {
                      return 0;  // IDLE
                    } else {
                      return 0;  // IDLE
                    }
                  }
                } else {
                  if (features[28] <= 441.889221f) {
                    return 4;  // BROWSE
                  } else {
                    return 0;  // IDLE
                  }
                }
              } else {
                if (features[17] <= 19888.104492f) {
                  return 3;  // MESSAGE
                } else {
                  if (features[9] <= 1421.033752f) {
                    if (features[23] <= 0.560840f) {
                      return 3;  // MESSAGE
                    } else {
                      if (features[13] <= 87.035000f) {
                        return 3;  // MESSAGE
                      } else {
                        return 0;  // IDLE
                      }
                    }
                  } else {
                    if (features[29] <= 476.867279f) {
                      return 0;  // IDLE
                    } else {
                      if (features[27] <= 17.322894f) {
                        return 3;  // MESSAGE
                      } else {
                        return 0;  // IDLE
                      }
                    }
                  }
                }
              }
            } else {
              if (features[8] <= 2.584355f) {
                return 3;  // MESSAGE
              } else {
                if (features[1] <= 22.565515f) {
                  if (features[16] <= 3032.662231f) {
                    if (features[24] <= 32.231001f) {
                      if (features[12] <= 24.398915f) {
                        return 3;  // MESSAGE
                      } else {
                        return 0;  // IDLE
                      }
                    } else {
                      return 4;  // BROWSE
                    }
                  } else {
                    return 4;  // BROWSE
                  }
                } else {
                  return 0;  // IDLE
                }
              }
            }
          } else {
            if (features[10] <= 1431.710022f) {
              if (features[15] <= 142545.125000f) {
                if (features[18] <= 144362.617188f) {
                  if (features[13] <= 78.974998f) {
                    return 3;  // MESSAGE
                  } else {
                    if (features[13] <= 83.404999f) {
                      if (features[27] <= 16.734881f) {
                        return 0;  // IDLE
                      } else {
                        return 1;  // SOCIAL
                      }
                    } else {
                      if (features[28] <= 394.659805f) {
                        if (features[4] <= 1469.484985f) {
                          return 0;  // IDLE
                        } else {
                          if (features[24] <= 16.305929f) {
                            return 4;  // BROWSE
                          } else {
                            return 0;  // IDLE
                          }
                        }
                      } else {
                        return 0;  // IDLE
                      }
                    }
                  }
                } else {
                  if (features[17] <= 19881.489258f) {
                    return 3;  // MESSAGE
                  } else {
                    if (features[13] <= 126.520004f) {
                      return 0;  // IDLE
                    } else {
                      return 3;  // MESSAGE
                    }
                  }
                }
              } else {
                return 3;  // MESSAGE
              }
            } else {
              if (features[0] <= 1423.545044f) {
                if (features[17] <= 19851.070312f) {
                  return 3;  // MESSAGE
                } else {
                  if (features[11] <= 17.008738f) {
                    if (features[28] <= 277.134651f) {
                      return 3;  // MESSAGE
                    } else {
                      if (features[12] <= 25.237809f) {
                        return 0;  // IDLE
                      } else {
                        return 0;  // IDLE
                      }
                    }
                  } else {
                    if (features[9] <= 1422.648743f) {
                      return 0;  // IDLE
                    } else {
                      return 3;  // MESSAGE
                    }
                  }
                }
              } else {
                if (features[4] <= 1451.354980f) {
                  return 4;  // BROWSE
                } else {
                  if (features[21] <= 1.211864f) {
                    return 3;  // MESSAGE
                  } else {
                    return 3;  // MESSAGE
                  }
                }
              }
            }
          }
        }
      } else {
        if (features[16] <= 3025.962280f) {
          if (features[6] <= 1431.007507f) {
            if (features[1] <= 19.747495f) {
              if (features[2] <= 277.269821f) {
                return 1;  // SOCIAL
              } else {
                if (features[19] <= 6164.587402f) {
                  if (features[7] <= -3.148034f) {
                    return 4;  // BROWSE
                  } else {
                    if (features[27] <= 14.904777f) {
                      return 4;  // BROWSE
                    } else {
                      if (features[26] <= 14.974384f) {
                        return 4;  // BROWSE
                      } else {
                        return 3;  // MESSAGE
                      }
                    }
                  }
                } else {
                  if (features[2] <= 379.786621f) {
                    if (features[12] <= 25.915152f) {
                      return 3;  // MESSAGE
                    } else {
                      return 3;  // MESSAGE
                    }
                  } else {
                    return 4;  // BROWSE
                  }
                }
              }
            } else {
              if (features[23] <= 0.609337f) {
                if (features[3] <= 1347.804993f) {
                  if (features[4] <= 1442.085022f) {
                    return 3;  // MESSAGE
                  } else {
                    if (features[27] <= 18.117161f) {
                      return 0;  // IDLE
                    } else {
                      return 0;  // IDLE
                    }
                  }
                } else {
                  if (features[25] <= 23.977507f) {
                    return 4;  // BROWSE
                  } else {
                    return 3;  // MESSAGE
                  }
                }
              } else {
                if (features[10] <= 1431.609985f) {
                  return 0;  // IDLE
                } else {
                  if (features[15] <= 142547.117188f) {
                    if (features[3] <= 1351.429993f) {
                      return 3;  // MESSAGE
                    } else {
                      return 4;  // BROWSE
                    }
                  } else {
                    return 0;  // IDLE
                  }
                }
              }
            }
          } else {
            if (features[9] <= 1427.078796f) {
              if (features[11] <= 13.333131f) {
                if (features[17] <= 19911.621094f) {
                  return 0;  // IDLE
                } else {
                  if (features[19] <= 5846.133789f) {
                    return 3;  // MESSAGE
                  } else {
                    if (features[7] <= -3.518493f) {
                      return 3;  // MESSAGE
                    } else {
                      return 3;  // MESSAGE
                    }
                  }
                }
              } else {
                if (features[0] <= 1425.245544f) {
                  if (features[0] <= 1424.911133f) {
                    if (features[25] <= 22.034439f) {
                      if (features[24] <= 23.646178f) {
                        if (features[0] <= 1424.717468f) {
                          if (features[1] <= 21.137201f) {
                            if (features[26] <= 27.978153f) {
                              return 3;  // MESSAGE
                            } else {
                              return 4;  // BROWSE
                            }
                          } else {
                            return 0;  // IDLE
                          }
                        } else {
                          return 4;  // BROWSE
                        }
                      } else {
                        return 4;  // BROWSE
                      }
                    } else {
                      if (features[18] <= 144432.476562f) {
                        if (features[1] <= 21.025502f) {
                          return 4;  // BROWSE
                        } else {
                          return 3;  // MESSAGE
                        }
                      } else {
                        return 3;  // MESSAGE
                      }
                    }
                  } else {
                    if (features[10] <= 1434.130005f) {
                      if (features[13] <= 107.985001f) {
                        if (features[28] <= 296.011887f) {
                          return 3;  // MESSAGE
                        } else {
                          return 3;  // MESSAGE
                        }
                      } else {
                        return 3;  // MESSAGE
                      }
                    } else {
                      return 4;  // BROWSE
                    }
                  }
                } else {
                  if (features[8] <= 10.736091f) {
                    if (features[12] <= 26.068702f) {
                      if (features[21] <= 1.307265f) {
                        if (features[12] <= 22.406758f) {
                          return 4;  // BROWSE
                        } else {
                          return 4;  // BROWSE
                        }
                      } else {
                        return 4;  // BROWSE
                      }
                    } else {
                      if (features[10] <= 1433.326233f) {
                        if (features[26] <= 18.521375f) {
                          return 4;  // BROWSE
                        } else {
                          return 3;  // MESSAGE
                        }
                      } else {
                        if (features[26] <= 16.413188f) {
                          return 4;  // BROWSE
                        } else {
                          return 4;  // BROWSE
                        }
                      }
                    }
                  } else {
                    if (features[24] <= 16.384473f) {
                      return 3;  // MESSAGE
                    } else {
                      return 4;  // BROWSE
                    }
                  }
                }
              }
            } else {
              if (features[21] <= 1.364628f) {
                if (features[10] <= 1432.516235f) {
                  if (features[16] <= 3008.763550f) {
                    return 3;  // MESSAGE
                  } else {
                    if (features[18] <= 144077.156250f) {
                      return 0;  // IDLE
                    } else {
                      return 4;  // BROWSE
                    }
                  }
                } else {
                  if (features[3] <= 1330.474976f) {
                    return 1;  // SOCIAL
                  } else {
                    if (features[8] <= 10.274560f) {
                      if (features[17] <= 19962.890625f) {
                        return 4;  // BROWSE
                      } else {
                        return 4;  // BROWSE
                      }
                    } else {
                      if (features[18] <= 144144.414062f) {
                        return 4;  // BROWSE
                      } else {
                        return 3;  // MESSAGE
                      }
                    }
                  }
                }
              } else {
                return 3;  // MESSAGE
              }
            }
          }
        } else {
          if (features[16] <= 3064.272583f) {
            if (features[27] <= 19.196944f) {
              if (features[25] <= 25.334064f) {
                if (features[15] <= 142437.484375f) {
                  if (features[10] <= 1432.921204f) {
                    if (features[9] <= 1416.501221f) {
                      if (features[4] <= 1442.085022f) {
                        return 3;  // MESSAGE
                      } else {
                        return 4;  // BROWSE
                      }
                    } else {
                      if (features[12] <= 26.450023f) {
                        if (features[19] <= 6493.085449f) {
                          if (features[15] <= 141885.906250f) {
                            return 3;  // MESSAGE
                          } else {
                            return 0;  // IDLE
                          }
                        } else {
                          return 3;  // MESSAGE
                        }
                      } else {
                        if (features[4] <= 1444.505005f) {
                          return 4;  // BROWSE
                        } else {
                          if (features[11] <= 13.992222f) {
                            return 4;  // BROWSE
                          } else {
                            if (features[13] <= 91.470001f) {
                              return 3;  // MESSAGE
                            } else {
                              return 3;  // MESSAGE
                            }
                          }
                        }
                      }
                    }
                  } else {
                    if (features[12] <= 28.930621f) {
                      if (features[15] <= 141221.062500f) {
                        return 3;  // MESSAGE
                      } else {
                        return 4;  // BROWSE
                      }
                    } else {
                      return 3;  // MESSAGE
                    }
                  }
                } else {
                  if (features[28] <= 192.647408f) {
                    return 3;  // MESSAGE
                  } else {
                    if (features[6] <= 1430.602539f) {
                      return 3;  // MESSAGE
                    } else {
                      if (features[27] <= 16.238756f) {
                        if (features[2] <= 424.317825f) {
                          return 3;  // MESSAGE
                        } else {
                          return 4;  // BROWSE
                        }
                      } else {
                        return 4;  // BROWSE
                      }
                    }
                  }
                }
              } else {
                if (features[4] <= 1472.710022f) {
                  if (features[24] <= 90.593163f) {
                    if (features[12] <= 27.553678f) {
                      return 4;  // BROWSE
                    } else {
                      if (features[26] <= 20.819643f) {
                        return 3;  // MESSAGE
                      } else {
                        return 4;  // BROWSE
                      }
                    }
                  } else {
                    return 3;  // MESSAGE
                  }
                } else {
                  if (features[10] <= 1433.326233f) {
                    if (features[17] <= 19912.654297f) {
                      return 4;  // BROWSE
                    } else {
                      if (features[21] <= 1.328151f) {
                        return 4;  // BROWSE
                      } else {
                        return 3;  // MESSAGE
                      }
                    }
                  } else {
                    return 4;  // BROWSE
                  }
                }
              }
            } else {
              if (features[5] <= 120.480000f) {
                if (features[9] <= 1418.919983f) {
                  return 4;  // BROWSE
                } else {
                  if (features[23] <= 0.642382f) {
                    return 4;  // BROWSE
                  } else {
                    return 3;  // MESSAGE
                  }
                }
              } else {
                if (features[15] <= 142207.492188f) {
                  return 3;  // MESSAGE
                } else {
                  if (features[18] <= 144045.296875f) {
                    if (features[5] <= 139.010002f) {
                      return 3;  // MESSAGE
                    } else {
                      return 0;  // IDLE
                    }
                  } else {
                    if (features[26] <= 19.808696f) {
                      if (features[2] <= 446.027252f) {
                        return 0;  // IDLE
                      } else {
                        return 3;  // MESSAGE
                      }
                    } else {
                      if (features[5] <= 143.445000f) {
                        if (features[17] <= 19894.701172f) {
                          return 4;  // BROWSE
                        } else {
                          if (features[13] <= 82.195000f) {
                            return 3;  // MESSAGE
                          } else {
                            return 4;  // BROWSE
                          }
                        }
                      } else {
                        if (features[19] <= 8397.511230f) {
                          if (features[10] <= 1433.224976f) {
                            return 3;  // MESSAGE
                          } else {
                            if (features[26] <= 28.419750f) {
                              return 4;  // BROWSE
                            } else {
                              return 3;  // MESSAGE
                            }
                          }
                        } else {
                          return 3;  // MESSAGE
                        }
                      }
                    }
                  }
                }
              }
            }
          } else {
            if (features[12] <= 35.218632f) {
              if (features[9] <= 1418.116211f) {
                if (features[23] <= 0.816277f) {
                  if (features[29] <= 436.501190f) {
                    if (features[2] <= 962.787109f) {
                      return 4;  // BROWSE
                    } else {
                      return 3;  // MESSAGE
                    }
                  } else {
                    if (features[0] <= 1413.734131f) {
                      if (features[0] <= 1411.268127f) {
                        return 1;  // SOCIAL
                      } else {
                        return 4;  // BROWSE
                      }
                    } else {
                      return 3;  // MESSAGE
                    }
                  }
                } else {
                  if (features[21] <= 1.391896f) {
                    return 4;  // BROWSE
                  } else {
                    return 3;  // MESSAGE
                  }
                }
              } else {
                if (features[2] <= 691.318512f) {
                  return 3;  // MESSAGE
                } else {
                  if (features[26] <= 22.807362f) {
                    if (features[24] <= 61.646021f) {
                      if (features[11] <= 14.216616f) {
                        return 1;  // SOCIAL
                      } else {
                        if (features[18] <= 146369.125000f) {
                          return 4;  // BROWSE
                        } else {
                          return 3;  // MESSAGE
                        }
                      }
                    } else {
                      return 3;  // MESSAGE
                    }
                  } else {
                    if (features[11] <= 15.070909f) {
                      if (features[21] <= 1.382891f) {
                        return 4;  // BROWSE
                      } else {
                        return 3;  // MESSAGE
                      }
                    } else {
                      return 4;  // BROWSE
                    }
                  }
                }
              }
            } else {
              return 5;  // GAMING
            }
          }
        }
      }
    } else {
      if (features[17] <= 19652.078125f) {
        if (features[24] <= 60.240459f) {
          return 4;  // BROWSE
        } else {
          if (features[18] <= 146469.750000f) {
            if (features[28] <= 272.339767f) {
              return 0;  // IDLE
            } else {
              if (features[5] <= 172.854996f) {
                if (features[13] <= 91.865002f) {
                  if (features[21] <= 1.579678f) {
                    return 1;  // SOCIAL
                  } else {
                    if (features[6] <= 1427.179993f) {
                      if (features[9] <= 1355.355042f) {
                        return 1;  // SOCIAL
                      } else {
                        return 1;  // SOCIAL
                      }
                    } else {
                      return 4;  // BROWSE
                    }
                  }
                } else {
                  if (features[21] <= 1.540912f) {
                    if (features[9] <= 1355.760010f) {
                      return 1;  // SOCIAL
                    } else {
                      if (features[1] <= 37.232479f) {
                        return 1;  // SOCIAL
                      } else {
                        return 3;  // MESSAGE
                      }
                    }
                  } else {
                    if (features[18] <= 142896.054688f) {
                      return 1;  // SOCIAL
                    } else {
                      return 1;  // SOCIAL
                    }
                  }
                }
              } else {
                if (features[5] <= 173.659996f) {
                  return 0;  // IDLE
                } else {
                  return 1;  // SOCIAL
                }
              }
            }
          } else {
            return 1;  // SOCIAL
          }
        }
      } else {
        if (features[17] <= 19697.824219f) {
          if (features[18] <= 146260.195312f) {
            if (features[19] <= 7605.632812f) {
              return 3;  // MESSAGE
            } else {
              if (features[18] <= 145732.976562f) {
                return 3;  // MESSAGE
              } else {
                if (features[11] <= 17.866818f) {
                  return 1;  // SOCIAL
                } else {
                  if (features[5] <= 138.205002f) {
                    return 4;  // BROWSE
                  } else {
                    return 1;  // SOCIAL
                  }
                }
              }
            }
          } else {
            if (features[26] <= 28.396122f) {
              return 4;  // BROWSE
            } else {
              return 3;  // MESSAGE
            }
          }
        } else {
          if (features[18] <= 146036.093750f) {
            if (features[23] <= 0.873915f) {
              if (features[25] <= 33.815174f) {
                if (features[27] <= 20.673238f) {
                  if (features[13] <= 94.285000f) {
                    return 3;  // MESSAGE
                  } else {
                    return 4;  // BROWSE
                  }
                } else {
                  return 3;  // MESSAGE
                }
              } else {
                return 3;  // MESSAGE
              }
            } else {
              return 1;  // SOCIAL
            }
          } else {
            if (features[1] <= 35.456854f) {
              if (features[3] <= 1344.575012f) {
                if (features[28] <= 266.384132f) {
                  return 3;  // MESSAGE
                } else {
                  if (features[11] <= 20.268182f) {
                    if (features[26] <= 20.690195f) {
                      return 3;  // MESSAGE
                    } else {
                      if (features[2] <= 1121.666077f) {
                        if (features[9] <= 1404.717468f) {
                          return 1;  // SOCIAL
                        } else {
                          return 4;  // BROWSE
                        }
                      } else {
                        return 4;  // BROWSE
                      }
                    }
                  } else {
                    return 3;  // MESSAGE
                  }
                }
              } else {
                return 3;  // MESSAGE
              }
            } else {
              return 3;  // MESSAGE
            }
          }
        }
      }
    }
  }
} else {
  if (features[10] <= 1434.534973f) {
    if (features[19] <= 8459.102783f) {
      return 4;  // BROWSE
    } else {
      return 1;  // SOCIAL
    }
  } else {
    if (features[21] <= 1.857897f) {
      return 4;  // BROWSE
    } else {
      return 5;  // GAMING
    }
  }
}

}

#endif  // RANDOM_FOREST_MODEL_H
