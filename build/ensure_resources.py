import os
import shutil
import glob

def ensure_resources():

    source_dir = "resource"
    dest_dir = "data"

    pattern = os.path.join(source_dir, "*.example.*")

    examples = glob.glob(pattern, recursive=True)
    for source_filename in examples:

        # Strip out the '.example' part
        filename = os.path.basename(source_filename)
        target_filename = filename.replace(".example.", ".")
        target_path = os.path.join(dest_dir, target_filename)

        # don't overwrite existing resources
        if os.path.exists(target_path):
            continue

        shutil.copy(source_filename, target_path)
    # end for
# end function

ensure_resources()
