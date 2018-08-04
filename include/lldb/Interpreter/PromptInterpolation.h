//===-- PromptInterpolation.h --------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_PromptInterpolation_h_
#define liblldb_PromptInterpolation_h_

#include <string>

namespace lldb_private {
class Debugger;
}

namespace lldb_private {

class PromptInterpolation {
public:
  PromptInterpolation(Debugger& debugger) :
    m_debugger(debugger) {
  }

  std::string InterpolatePrompt(const std::string& prompt_format);
private:
  Debugger &m_debugger;
};

} // namespace lldb_private

#endif // liblldb_PromptInterpolation_h_

