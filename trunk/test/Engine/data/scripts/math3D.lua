--vec3:new(x,y,z) - конструктор вектора
--vec3:length - получить длину вектора
--vec3:normalize - нормализовать вектор
--vec3:set_length(newlength) - установить длину вектора
--vec3+vec3 - сложение векторов
--vec3-vec3 - вычитание векторов
--vec3*vec3 - векторное умножение векторов

mat3x3 = {}
mat3x3.mt = {__index = mat3x3}

function mat3x3:new()
  local r = {}
  setmetatable(r,mat3x3.mt)
  r.m11 = 1;
  r.m12 = 0;
  r.m13 = 0;
  r.m21 = 0;
  r.m22 = 1;
  r.m23 = 0;
  r.m31 = 0;
  r.m32 = 0;
  r.m33 = 1;
  return r;
end

function mat3x3:set(a)
  self.m11 = a.m11;
  self.m12 = a.m12;
  self.m13 = a.m13;
  self.m21 = a.m21;
  self.m22 = a.m22;
  self.m23 = a.m23;
  self.m31 = a.m31;
  self.m32 = a.m32;
  self.m33 = a.m33;
end

mat3x3.mt.__mul = function(a,b)
  local res = mat3x3:new();
  res.m11=a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31;
  res.m12=a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32;
  res.m13=a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33;

  res.m21=a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31;
  res.m22=a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32;
  res.m23=a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33;

  res.m31=a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31;
  res.m32=a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32;
  res.m33=a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33;
  return res;
end

function mat3x3:addXrotate(angle)
  local s = math.sin(angle/180*3.14);
  local c = math.cos(angle/180*3.14);
  local tempMatrix = mat3x3:new();
  
 tempMatrix.m11=1;
 tempMatrix.m12=0;
 tempMatrix.m13=0;

 tempMatrix.m21=0;
 tempMatrix.m22=c;
 tempMatrix.m23=s;

 tempMatrix.m31=0;
 tempMatrix.m32=-s;
 tempMatrix.m33=c;  
  
  self:set(tempMatrix*self);
end

function mat3x3:addYrotate(angle)
  local s = math.sin(angle/180*3.14);
  local c = math.cos(angle/180*3.14);
  local tempMatrix = mat3x3:new();
  
 tempMatrix.m11=c;
 tempMatrix.m12=0;
 tempMatrix.m13=-s;

 tempMatrix.m21=0;
 tempMatrix.m22=1;
 tempMatrix.m23=0;

 tempMatrix.m31=s;
 tempMatrix.m32=0;
 tempMatrix.m33=c;
  
  self:set(tempMatrix*self);
end

function mat3x3:addZrotate(angle)
  local s = math.sin(angle/180*3.14);
  local c = math.cos(angle/180*3.14);
  local tempMatrix = mat3x3:new();
  
 tempMatrix.m11=c;
 tempMatrix.m12=s;
 tempMatrix.m13=0;

 tempMatrix.m21=-s;
 tempMatrix.m22=c;
 tempMatrix.m23=0;

 tempMatrix.m31=0;
 tempMatrix.m32=0;
 tempMatrix.m33=1;
  
  self:set(tempMatrix*self);
end

vec3 = {x=0, y=0, z=0};
vec3.mt = {__index = vec3};
vec3.mt.__add = function(a,b)
  local res = vec3:new(0,0,0);
  res.x = a.x + b.x;
  res.y = a.y + b.y;
  res.z = a.z + b.z;
  return res;
end

vec3.mt.__sub = function(a,b)
  local res = vec3:new(0,0,0)
  res.x = a.x - b.x;
  res.y = a.y - b.y;
  res.z = a.z - b.z;
  return res;
end

vec3.mt.__mul = function(a,b)
  local res = vec3:new(0,0,0);
  res.x =  (a.y  * b.z) - (a.z  * b.y);
  res.y = (a.z  * b.x) - (a.x  * b.z);
  res.z = (a.x  * b.y) - (a.y  * b.x);
  return res;
end

vec3.mt.__eq = function(a,b)
  return math.abs(a.x-b.x)<0.0000001 and math.abs(a.y-b.y)<0.0000001 and math.abs(a.z-b.z)<0.0000001;
end

vec3.mt.__lt = function(a,b)
  return (a.x*a.x + a.y*a.y + a.z*a.z)<(b.x*b.x + b.y*b.y + b.z*b.z)
end

vec3.mt.__le = function(a,b)
  return (a.x*a.x + a.y*a.y + a.z*a.z)<(b.x*b.x + b.y*b.y + b.z*b.z)
end

function vec3:new (x,y,z)
  local r = {}
  setmetatable(r,vec3.mt)
  r.x = x
  r.y = y
  r.z = z  
  return r
end

function vec3:length()
  return math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z)
end

function vec3:normalize()
  local lng = self:length()
  if (lng~=0) then
    self.x = self.x/lng
    self.y = self.y/lng
    self.z = self.z/lng
  end	  
end

function vec3:set_length(newlength)
  local lng = self:length()
  if (lng~=0) then
    self.x = self.x/lng*newlength
    self.y = self.y/lng*newlength
    self.z = self.z/lng*newlength
  end	  
end

function vec3:set_scale(scale)
  self.x = self.x*scale
  self.y = self.y*scale
  self.z = self.z*scale
end

function vec3:get_components()
  return self.x,self.y,self.z
end

function vec3:dot(anothervector)
  return ((self.x* anothervector.x) + (self.y* anothervector.y) + (self.z* anothervector.z));
end

function vec3:reflect(normal)
  local v = vec3:new(self.x,self.y,self.z)
  v:normalize()
  
  local pos = normal;
  pos:normalize()
  pos:set_length(math.cos(math.atan(self:dot(normal))))
  
  pos = pos + pos -v;
  --pos = normal;
  pos:set_length(self:length())
  self.x = pos.x
  self.y = pos.y
  self.z = pos.z
end

function sqr(a)
  return a*a;
end

function vec3:distance(a)
  return math.sqrt(sqr(self.x-a.x) + sqr(self.y-a.y) + sqr(self.z-a.z))
end

function vec3:angle(b)
	local cos = self:dot(b);
	-- important to be '< 1' because of math.acos bug with float tiny > 1
	if cos < 1 then 
		return math.deg(math.acos(cos));
	end;
	return 0;
end;

function vec3:norm_project(b)
	return vec3:new(self.x * b.x, self.y * b.y, self.z * b.z);
end;

function vec3:set(v)
  self.x = v.x;
  self.y = v.y;
  self.z = v.z;
end


function distance_to_plane_vec3(point,normal,planevertex)
  return -(normal.x * (point.x-planevertex.x) + normal.y * (point.y-planevertex.y) + normal.z * (point.z-planevertex.z))
end

function sign(a)
  if (a<0) then
    return -1;
  end;
  if (a>0) then
    return 1;
  end;
  return 0;
end;

function rotate2D(x,y,angle)
  local degrees = -angle/180*3.14;
  return (x*math.cos(degrees)-y*math.sin(degrees)),
         (x*math.sin(degrees)+y*math.cos(degrees));
end;
