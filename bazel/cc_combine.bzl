load("@bazel_tools//tools/cpp:toolchain_utils.bzl", "find_cpp_toolchain")

def _combine_impl(ctx):
    cc_toolchain = find_cpp_toolchain(ctx)   
    print("ar: {}".format(cc_toolchain.ar_executable))
    target_list = []
    for dep_target in ctx.attr.deps:        
        # CcInfo, InstrumentedFilesInfo, OutputGroupInfo    
        # Refer https://docs.bazel.build/versions/main/skylark/lib/LinkerInput.html  
        cc_info_linker_inputs = dep_target[CcInfo].linking_context.linker_inputs
        for linker_in in cc_info_linker_inputs.to_list():            
            for linker_in_lib in linker_in.libraries:     
                # Refer https://docs.bazel.build/versions/main/skylark/lib/LibraryToLink.html           
                if linker_in_lib.pic_static_library != None:
                    print("Add PIC archive {}".format(linker_in_lib.pic_static_library.path))
                    target_list.append(linker_in_lib.pic_static_library)
                else:
                    if linker_in_lib.static_library != None:
                        print("PIC of {} is missing".format(linker_in_lib.static_library.basename))
    
    output = ctx.outputs.output
    if ctx.attr.genstatic:
        cp_command  = ""       
        processed_list = []
        processed_path_list = []
        for dep in target_list:
            cp_command += "cp -a " + dep.path + " " + output.dirname + "/ && "
            processed = ctx.actions.declare_file(dep.basename)
            processed_list.append(processed)
            processed_path_list.append(dep.path)
        cp_command += "echo 'starting to run shell'"
        processed_path_list.append(output.path)
        ctx.actions.run_shell(
            outputs = processed_list,
            inputs = target_list,
            command = cp_command,
        )

        inflate_list = []
        inflate_command = "cd {} && ".format(output.dirname)
        for archive in processed_list:
            dir_name = archive.basename.rstrip(".a") + "_inflate"
            inflate_dir = ctx.actions.declare_directory(dir_name)                        
            inflate_list.append(inflate_dir)
            inflate_path = output.dirname + "/" + dir_name
            inflate_command += " cd {} && {} -x ../{} && cd .. && ".format(dir_name, cc_toolchain.ar_executable, archive.basename)
        inflate_command += "echo 'Create inflate directories'"
        ctx.actions.run_shell(
            outputs = inflate_list,
            inputs = processed_list,
            command = inflate_command,
        )

        command = "cd {} && {} -qc {} ".format(output.dirname, cc_toolchain.ar_executable, output.basename)
        for item in inflate_list:
            command += " {}/*.o ".format(item.basename)
        ctx.actions.run_shell(
            outputs = [output],
            inputs = inflate_list,
            command = command,
        )
    else:
        command = "export PATH=$PATH:{} && {} -shared -fPIC -Wl,--whole-archive {} -Wl,--no-whole-archive -Wl,-soname -o {}".format(
            cc_toolchain.ld_executable,
            cc_toolchain.compiler_executable,
            " ".join([dep.path for dep in target_list]),
            output.path)
        print("command = ", command)
        ctx.actions.run_shell(
            outputs = [output],
            inputs = target_list,
            command = command,
        )

cc_combine = rule(
    implementation = _combine_impl,
    attrs = {
        "_cc_toolchain": attr.label(default = Label("@bazel_tools//tools/cpp:current_cc_toolchain")),
        "genstatic" : attr.bool(default = False),
        "deps": attr.label_list(allow_files = [".a"]),
        "output": attr.output()
    },
)