" Vim global plugin for semantic highlighting using libclang
" Maintainer: Jeaye <contact@jeaye.com>

"hi link ColorCodeType                      Type
hi ColorCodeType ctermfg=Blue guifg=#164f99
hi link StructDecl                         ColorCodeType
hi link UnionDecl                          ColorCodeType
hi link ClassDecl                          ColorCodeType
hi link EnumDecl                           ColorCodeType
hi link TypedefDecl                        ColorCodeType
hi link TemplateTypeParameter              ColorCodeType
hi link TemplateTemplateParameter          ColorCodeType
hi link ClassTemplatePartialSpecialization ColorCodeType
hi link ClassTemplate                      ColorCodeType
hi link UsingDirective                     ColorCodeType
hi link UsingDeclaration                   ColorCodeType
hi link TypeAliasDecl                      ColorCodeType
hi link TypeRef                            ColorCodeType
hi link CXXBaseSpecifier                   ColorCodeType
hi link TemplateRef                        ColorCodeType

hi link ColorCodeFunction                  Function
hi link FunctionDecl                       ColorCodeFunction
hi link CXXMethod                          ColorCodeFunction
hi link Constructor                        ColorCodeFunction
hi link Destructor                         ColorCodeFunction
hi link ConversionFunction                 ColorCodeFunction
hi link FunctionTemplate                   ColorCodeFunction
hi link OverloadedDeclRef                  ColorCodeFunction
hi link CallExpr                           ColorCodeFunction

hi link ColorCodeNormal                    Normal
hi link ObjCInterfaceDecl                  ColorCodeNormal
hi link ObjCCategoryDecl                   ColorCodeNormal
hi link ObjCProtocolDecl                   ColorCodeNormal
hi link ObjCPropertyDecl                   ColorCodeNormal
hi link ObjCIvarDecl                       ColorCodeNormal
hi link ObjCInstanceMethodDecl             ColorCodeNormal
hi link ObjCClassMethodDecl                ColorCodeNormal
hi link ObjCImplementationDecl             ColorCodeNormal
hi link ObjCCategoryImplDecl               ColorCodeNormal
hi link LinkageSpec                        ColorCodeNormal
hi link ObjCSynthesizeDecl                 ColorCodeNormal
hi link ObjCDynamicDecl                    ColorCodeNormal
hi link ObjCSuperClassRef                  ColorCodeNormal
hi link ObjCProtocolRef                    ColorCodeNormal
hi link ObjCClassRef                       ColorCodeNormal
hi link FirstInvalid                       ColorCodeNormal
hi link NotImplemented                     ColorCodeNormal
hi link FirstExpr                          ColorCodeNormal
hi link ObjCMessageExpr                    ColorCodeNormal
hi link BlockExpr                          ColorCodeNormal
hi link Punctuation                        ColorCodeNormal

hi ColorCodeVariable ctermfg=Grey guifg=Grey
hi link Variable                           ColorCodeVariable
hi link VarDecl                            ColorCodeVariable
hi link ParmDecl                           ColorCodeVariable
hi link NonTypeTemplateParameter           ColorCodeVariable
hi link VariableRef                        ColorCodeVariable
hi link DeclRefExpr                        ColorCodeVariable

hi ColorCodeMember ctermfg=LightBlue guifg=LightBlue
hi link Member                             ColorCodeMember
hi link FieldDecl                          ColorCodeMember
hi link MemberRef                          ColorCodeMember
hi link MemberRefExpr                      ColorCodeMember

hi ColorCodeEnumConstant ctermfg=Green guifg=Green
hi link EnumConstant                      ColorCodeEnumConstant
hi link EnumConstantDecl                  ColorCodeEnumConstant

hi ColorCodeNamespace ctermfg=Yellow guifg=#FF0000
hi link Namespace                         ColorCodeNamespace
hi link NamespaceAlias                    ColorCodeNamespace
hi link NamespaceRef                      ColorCodeNamespace

hi link ColorCodeLabel                    Label
hi link CXXAccessSpecifier                ColorCodeLabel
hi link LabelRef                          ColorCodeLabel

hi link ColorCodeError                    Error
hi link InvalidFile                       ColorCodeError
hi link NoDeclFound                       ColorCodeError
hi link InvalidCode                       ColorCodeError

hi link ColorCodeMacro                    Macro
hi link MacroDefinition                   ColorCodeMacro
hi link MacroInstantiation                ColorCodeMacro

hi link ColorCodeNumber                   Number
hi link IntegerLiteral                    ColorCodeNumber
hi link ImaginaryLiteral                  ColorCodeNumber

hi link ColorCodeFloat                    Float
hi link FloatingLiteral                   ColorCodeFloat

hi link ColorCodeString                   String
hi link StringLiteral                     ColorCodeString

hi link ColorCodeCharacter                Character
hi link CharacterLiteral                  ColorCodeCharacter


