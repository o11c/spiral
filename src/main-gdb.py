class VecPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return self.val['data']

def vec_printer(val):
    type = gdb.types.get_basic_type(val.type)
    name = type.tag
    if name is None:
        return None
    if name.startswith('i'):
        name = name[1:]
    if name[-1] in '234':
        name = name[:-1]
    if name in ('vec', 'quat', 'mat'):
        return VecPrinter(val)
    return None

gdb.current_objfile().pretty_printers.append(vec_printer)
