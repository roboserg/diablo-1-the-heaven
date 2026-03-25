import os
import sys
import io
import argparse

def dep_path(path):
    return os.path.join(path, "dependencies.txt")


def target_path(path):
    return os.path.join(path, "targets_path.txt")


def reset_deps(args):
    io.open(dep_path(args.path), mode="w").close()
    io.open(target_path(args.path), mode="w").close()


def store_dep(args):
    with io.open(dep_path(args.path), mode="ab") as fout:
        fout.write("{}\t{}\n".format(args.current_target, args.dep_on_path))


def store_target_path(args):
    with io.open(target_path(args.path), mode="ab") as fout:
        fout.write("{}\t{}\n".format(args.target_name, args.target_path))


def find_target_by_path(args):
    with io.open(target_path(args.path), mode="rb") as fin:
        for line in fin:
            target, path = line.split('\t', 1)
            if path.strip() == args.target_path.strip():
                print target.strip()
                return


def list_all_targets(args):
    targets = set()
    with io.open(target_path(args.path), mode="rb") as fin:
        for line in fin:
            target, _ = line.split('\t', 1)
            targets.add(target.strip())

    for t in targets:
        print t


def list_all_deps_for_target(args):
    paths = dict()
    with io.open(target_path(args.path), mode="rb") as fin:
        for line in fin:
            target, path = line.split('\t', 1)
            target = target.strip()
            path = path.strip()
            paths[path] = target

    deps = set()
    with io.open(dep_path(args.path), mode="rb") as fin:
        for line in fin:
            target, path = line.split('\t', 1)
            target = target.strip()
            path = path.strip()
            if target == args.target.strip():
                if path in paths:
                    deps.add(paths[path])
                else:
                    print >>sys.stderr, "wrong target {}".format(args.target)
                    sys.exit(1)

    for d in deps:
        print d


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers()
    parser_1 = subparsers.add_parser('reset_deps')
    parser_1.add_argument('--path', type=str)
    parser_1.set_defaults(func=reset_deps)

    parser_2 = subparsers.add_parser('store_dep')
    parser_2.add_argument('--path', type=str)
    parser_2.add_argument('--current-target', type=str)
    parser_2.add_argument('--dep-on-path', type=str)
    parser_2.set_defaults(func=store_dep)

    parser_3 = subparsers.add_parser('store_target_path')
    parser_3.add_argument('--path', type=str)
    parser_3.add_argument('--target-name', type=str)
    parser_3.add_argument('--target-path', type=str)
    parser_3.set_defaults(func=store_target_path)

    parser_4 = subparsers.add_parser('find_target_by_path')
    parser_4.add_argument('--path', type=str)
    parser_4.add_argument('--target-path', type=str)
    parser_4.set_defaults(func=find_target_by_path)

    parser_5 = subparsers.add_parser('list_all_targets')
    parser_5.add_argument('--path', type=str)
    parser_5.set_defaults(func=list_all_targets)

    parser_6 = subparsers.add_parser('list_all_deps_for_target')
    parser_6.add_argument('--path', type=str)
    parser_6.add_argument('--target', type=str)
    parser_6.set_defaults(func=list_all_deps_for_target)

    args = parser.parse_args(sys.argv[1:])
    args.func(args)

if __name__ == "__main__":
    main()
