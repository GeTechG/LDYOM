
from cxxheaderparser import simple
from cxxheaderparser import types

def specialization_to_string(spec: types.TemplateSpecialization):
    specs = []
    for arg in spec.args:
        if isinstance(arg.arg, types.DecoratedType):
            specs.append(pqname_to_string(arg.arg.typename));
        elif isinstance(arg.arg, types.FunctionType):
            specs.append(func_type_to_string(arg.arg));
        else:
            pass;
    return ','.join(specs);

def func_type_to_string(func_: types.FunctionType):
    ret_type = pqname_to_string(func_.return_type.typename);
    params = [];
    for param in func_.parameters:
        params.append(pqname_to_string(param.type.typename));
    return '{}({})'.format(ret_type, ','.join(params));

def pqname_to_string(name: types.PQName):
    type_segments = [];
    for segment in name.segments:
        spec = "";
        if isinstance(segment, types.NameSpecifier):
            if segment.specialization != None:
                spec = "<{}>".format(specialization_to_string(segment.specialization));
        type_segments.append(segment.name + spec);
    return '::'.join(type_segments)

def parse_decorate_type(dec: types.DecoratedType | None):
    type: types.PQName | types.Any;
    if isinstance(dec, types.Pointer):
        type = parse_ptr_ref(dec.ptr_to)
    elif isinstance(dec, types.Reference):
        type = parse_ptr_ref(dec.ref_to)
    elif isinstance(dec, types.MoveReference):
        type = parse_ptr_ref(dec.moveref_to)  
    elif isinstance(dec, types.Array):
        type = parse_ptr_ref(dec.array_of)     
    else:
        type = dec.typename
    return type;

def parse_ptr_ref(ptr_ref: types.typing.Union[types.Array, types.FunctionType, types.Pointer, types.Type]):
    if isinstance(ptr_ref, types.FunctionType):
        return parse_decorate_type(ptr_ref.return_type);
    else:
        return ptr_ref.typename;

def parseHeader(path: str):
    data = simple.parse_file(path)
    classes = [];
    for clazz in data.namespace.classes:
        fields = []
        methods = [];
        constructors = [];
        for field in clazz.fields:
            if field.access == "public":
                valueType = parse_decorate_type(field.type);
                fields.append({
                    "name": field.name,
                    "valueType": pqname_to_string(valueType)
                });
        for method in clazz.methods:
            if method.access == "public":
                return_type: str = "void" if method.return_type == None else pqname_to_string(parse_decorate_type(method.return_type));

                try:
                    if method.return_type.const:
                        return_type = 'const {}'.format(return_type);
                except Exception as e:
                    pass

                if isinstance(method.return_type, types.Pointer):
                    return_type += "*";

                params = [];
                for param in method.parameters:
                    type: str = pqname_to_string(parse_decorate_type(param.type));

                    if isinstance(param.type, types.Pointer):
                        type += "*";

                    params.append({
                        "name": param.name,
                        "type": type
                    })

                method_obj = {
                    "name": pqname_to_string(method.name),
                    "return_type": return_type,
                    "static": method.static,
                    "params": params
                };
                if method.constructor:
                    constructors.append(method_obj);
                else:
                    methods.append(method_obj);
        classes.append({
            "name": pqname_to_string(clazz.class_decl.typename),
            "fields": fields,
            "methods": methods,
            "constructors": constructors
        })
    
    return classes;