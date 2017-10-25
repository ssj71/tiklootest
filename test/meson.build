project('tikloo demos', 'c')

subdir('tikloo')

executable( 'test_ui',
            'test.c',
            link_with : tikloo,
            dependencies : tk_deps
            )

executable( 'unittest',
            'unittest.c',
            link_with : tikloo,
            dependencies : tk_deps
            )
