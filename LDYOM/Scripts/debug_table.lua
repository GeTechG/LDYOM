utp = {}

function utp.key_to_str ( k )
   if "string" == type( k ) and string.match( k, "^[_%a][_%a%d]*$" ) then
      return k
   end
   return "[" .. utp.val_to_str( k ) .. "]"
end

function utp.val_to_str ( v )
   if "string" == type( v ) then
      v = string.gsub( v, "\n", "\\n" )
      if string.match( string.gsub(v,"[^'\"]",""), '^"+$' ) then
         return "'" .. v .. "'"
      end
      return '"' .. string.gsub(v,'"', '\\"' ) .. '"'
   end
   return "table" == type( v ) and utp.tostring( v ) or tostring( v )
end

function utp.tostring( tbl )
   local result, done = {}, {}
   for k, v in ipairs( tbl ) do
      table.insert( result, utp.val_to_str( v ) )
      done[ k ] = true
   end
   for k, v in pairs( tbl ) do
      if not done[ k ] then
         table.insert( result, utp.key_to_str( k ) .. "=" .. utp.val_to_str( v ) )
      end
   end
   return "{" .. table.concat( result, "," ) .. "}"
end

return utp
