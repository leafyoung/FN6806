## github - replit

rm main main.o main-debug
mkdir 4c-variant_visit_cxx20
mv *.h *.cpp 4c-variant_visit_cxx20/

git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 4c-variant_visit_cxx20
git fetch
git pull origin main

git add 4c-variant_visit_cxx20
git commit -m "4c-variant_visit_cxx20"
git rebase origin/main
git push -u origin main
