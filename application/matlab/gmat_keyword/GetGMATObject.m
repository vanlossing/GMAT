%$Header: /GMAT/dev/cvs/supportfiles/matlab/gmat_keyword/GetGMATObject.m,v 1.1 2007/08/22 19:15:16 shughes Exp $
function objct = GetGMATObject(var)

global gmatChannel;
%disp(['Get GMAT::' var]);

if (gmatChannel == 0) 
   disp('channel is not valid');
else
   var1 = [var  '.'];
   data = Request(gmatChannel, var1);
   eval(data);
   objct = eval(var);
end