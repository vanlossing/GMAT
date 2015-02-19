    doy           = 153;
    sec           = 43200;
    geolat        =  -0.0640194327055501;
    lon           =  2.76238489808435;
    height        =  360.013230194988
    lst           = (sec/3600 + lon*180/pi/15);
    F107A         = 150;
    F107          = 150;
    Ap            = 15*ones(1,7);
    [T, rho]      = atmosnrlmsise00(height*1000,geolat*180/pi,lon*180/pi,2004,doy,sec,lst,F107A,F107,Ap);
    rho           = rho(6)*1e9