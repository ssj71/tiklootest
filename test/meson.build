project('tikloo demos', 'c')

subdir('tikloo')

executable( 'test_ui',
            'test.c',
            link_with : tikloo,
            link_args : '-lm',
            dependencies : tk_deps
            )

executable( 'strtest',
            'strtest.c',
            link_with : tikloo,
            link_args : '-lm',
            dependencies : tk_deps
            )

executable( 'unittest',
            'unittest.c',
            link_with : tikloo,
            link_args : '-lm',
            dependencies : tk_deps
            )
