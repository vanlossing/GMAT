function a = asin(a)
%ASIN         Gradient inverse sine asin(a)
%

% written  10/16/98     S.M. Rump
% modified 10/14/00     S.M. Rump  use Tony's trick
% modified 04/04/04     S.M. Rump  set round to nearest for safety
%                                    accelaration for sparse input
% modified 04/06/05     S.M. Rump  rounding unchanged
% modified 11/20/05     S.M. Rump  fast check for rounding to nearest
% modified 09/28/08     S.M. Rump  check for rounding to nearest improved
% modified 10/08/08     S.M. Rump  improved sparse multiplication: not using intval data type
%

  e = 1e-30;
  if 1+e==1-e                           % fast check for rounding to nearest
    rndold = 0;
  else
    rndold = getround;
    setround(0)
  end

  global INTLAB_GRADIENT_NUMVAR
  N = INTLAB_GRADIENT_NUMVAR;

  % use full(a.x(:)): cures Matlab V6.0 bug
  % a=7; i=[1 1]; x=a(i), b=sparse(a); y=b(i)  yields row vector x but column vector y
  % ax is full anyway
  ax = 1 ./ sqrt( 1 - sqr(full(a.x(:))) );
  a.x = asin(a.x);
  if issparse(a.dx)
    sizeax = size(a.dx,1);
    [ia,ja,sa] = find(a.dx);
    if isa(a.x,'intval')
      adx = times(ax(ia),sa,0);
      if adx.complex
        a.dx = intval( sparse(ia,ja,adx.mid,sizeax,N) , sparse(ia,ja,adx.rad,sizeax,N) , 'midrad' );
      else
        a.dx = intval( sparse(ia,ja,adx.inf,sizeax,N) , sparse(ia,ja,adx.sup,sizeax,N) , 'infsup' );
      end
    else
      a.dx = sparse(ia,ja,ax(ia).*sa,sizeax,N);
    end
  else
    a.dx = a.dx .* ax(:,ones(1,N));
  end

  if rndold  
    setround(rndold)
  end
