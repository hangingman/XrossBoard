@echo ==========================================================================
@echo JaneClone�p�̃v���W�F�N�g�t�@�C�������܂��B
@echo ==========================================================================

cd %~dp0
@del *.vcxproj   >NUL 2>&1
@del *.filters   >NUL 2>&1
@del *.sln       >NUL 2>&1
bakefile-1.2.5.1_beta-win\bkl.exe .\janeclone.bkl

@echo ==========================================================================
@echo ���莝����VisualStudio��janeclone.sln���J���Ă��������B
@echo ==========================================================================

@pause