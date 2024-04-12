import Link from "@docusaurus/Link";
import styles from "@site/src/pages/download/index.module.css";
import VerInfoLDYOM from "@site/src/pages/download/verInfoLDYOM";

function Beta0_8_0_Preview_10() {
    return (<VerInfoLDYOM version="Beta 0.8.0 Preview 10" date="12.04.24"
                         changes={
                            <ul>
                                <li>Added support for tuning configuration for vehicles</li>
                                <li>Additional settings for vehicles have been added</li>
                                <li>Added player selection in the cutscene for tracking and linking</li>
                                <li>Redesigned cutscene node for the new camera</li>
                                <li>Rendering performance has been optimized</li>
                                <li>Added target parameter "set vehicle characteristics"</li>
                                <li>Carrec paths support added</li>
                                <li>Created camera extension and improved camera control in the cutscene</li>
                                <li>Camera paths have been added</li>
                            </ul>
                        }
                         supportOld={false}
                         buttons={[
                            <Link to="https://github.com/GeTechG/LDYOM/releases/tag/Beta_0.8_preview_10"
                                  className={styles.button}>Download</Link>
                        ]}
                         blogLink="2024/04/12/beta-0-8-0-preview-10"/>);
}

export default Beta0_8_0_Preview_10;